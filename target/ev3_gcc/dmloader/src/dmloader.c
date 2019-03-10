#include "kernel/kernel_impl.h"
#include "syssvc/syslog.h"
#include <kernel_cfg.h>
#include <string.h>
#include "dmloader.h"
#include "../app/common/module_common.h"
#include "elf32.h"
#include "platform_interface_layer.h"
#include "kernel/dataqueue.h"
#include "ev3.h"

enum {
	LDM_CAN_FREE,
	LDM_CAN_RUNNING,
};

typedef struct {
    ID     domid;
    void*  text_mempool;
    uint32_t   text_mempool_size;
    void*  data_mempool;
    uint32_t   data_mempool_size;
    int8_t status; // LDM_CAN_FREE, LDM_CAN_RUNNING
// Following only available when status == LDM_CAN_RUNNING
    MOD_CFG_ENTRY* cfg_table;
    uint32_t           cfg_entry_num;
} T_LDM_CAN; // Loadable Module Container

/**
 * Hard coded for EV3, should be more flexible in the future
 */
static STK_T app_text_mempool[COUNT_STK_T(TMAX_APP_TEXT_SIZE)] __attribute__((section(".app_text_mempool"),nocommon));
static STK_T app_data_mempool[COUNT_STK_T(TMAX_APP_DATA_SIZE)] __attribute__((section(".app_data_mempool"),nocommon));
static T_LDM_CAN ldm_cans[1];

void initialize_dmloader(intptr_t unused) {
	ldm_cans[0].status = LDM_CAN_FREE;
	ldm_cans[0].domid = TDOM_APP;
	ldm_cans[0].text_mempool = app_text_mempool;
	ldm_cans[0].text_mempool_size = sizeof(app_text_mempool);
	ldm_cans[0].data_mempool = app_data_mempool;
	ldm_cans[0].data_mempool_size = sizeof(app_data_mempool);
}

static inline
void dmloader_instruction_memory_barrier() {
    // Memory barrier from DDI0198D page 9-5
    asm volatile("clean_loop:");                 /* Clean entire dcache */
    asm volatile("mrc p15, 0, r15, c7, c10, 3");
    asm volatile("bne clean_loop");
    asm volatile("mcr p15, 0, r0, c7, c10, 4");  /* Drain write buffer */
    // TODO: str rx, [ry]
    asm volatile("mcr p15, 0, r0, c7, c5, 0");   /* Invalidate icache */
}

/**
 * Check whether a memory area is in an LDM container.
 */
inline static bool_t
probe_ldm_memory(const void *base, uint32_t size, T_LDM_CAN *ldm_can) {
	if(base >= ldm_can->text_mempool && base + size <= ldm_can->text_mempool + ldm_can->text_mempool_size)
		return true;
	if(base >= ldm_can->data_mempool && base + size <= ldm_can->data_mempool + ldm_can->data_mempool_size)
		return true;
	return false;
}

static ER
handle_module_cfg_tab(T_LDM_CAN *ldm_can) {
	// TODO: check cfg table memory
	assert(ldm_can->cfg_entry_num > 0 && ldm_can->cfg_table != NULL);
	assert(probe_ldm_memory(ldm_can->cfg_table, sizeof(MOD_CFG_ENTRY) * ldm_can->cfg_entry_num, ldm_can));

	ER_ID ercd = E_OK;

	// Creation stage
	for(uint32_t i = 0; i < ldm_can->cfg_entry_num && ercd == E_OK; ++i) {
		MOD_CFG_ENTRY *ent = &ldm_can->cfg_table[i];
		switch(ent->sfncd) {
		case TSFN_CRE_TSK: {
            syslog(LOG_DEBUG, "%s(): MOD_CFG_ENTRY TSFN_CRE_TSK", __FUNCTION__);
			assert(probe_ldm_memory(ent->argument, sizeof(T_CTSK), ldm_can));
			assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
			T_CTSK pk_ctsk = *(T_CTSK*)ent->argument;
			assert(probe_ldm_memory(pk_ctsk.stk, pk_ctsk.stksz, ldm_can)); // Check user stack
			assert(pk_ctsk.sstk == NULL);                                  // Check system stack
			pk_ctsk.tskatr &= ~TA_ACT;                                     // Clear TA_ACT
			assert(get_atrdomid(pk_ctsk.tskatr) == TDOM_SELF);             // Check original DOMID
			pk_ctsk.tskatr |= TA_DOM(ldm_can->domid);                      // Set new DOMID
			SVC_PERROR(ercd = acre_tsk(&pk_ctsk));
			if(ercd > 0) {
				// Store ID
			    *(ID*)ent->retvalptr = ercd;
#if defined(DEBUG) || 1
			    syslog(LOG_NOTICE, "%s(): Task (tid = %d) created.", __FUNCTION__, *(ID*)ent->retvalptr);
#endif

			    ercd = E_OK;
			}
			break; }

		case TSFN_CRE_SEM: {
		    syslog(LOG_DEBUG, "%s(): MOD_CFG_ENTRY TSFN_CRE_SEM", __FUNCTION__);
            assert(probe_ldm_memory(ent->argument, sizeof(T_CSEM), ldm_can));
            assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
            T_CSEM pk_csem = *(T_CSEM*)ent->argument;
			assert(get_atrdomid(pk_csem.sematr) == TDOM_SELF);             // Check original DOMID
			pk_csem.sematr |= TA_DOM(ldm_can->domid);                      // Set new DOMID
            ercd = acre_sem(&pk_csem);
            assert(ercd > 0);
            if(ercd > 0) {
                // Store ID
                *(ID*)ent->retvalptr = ercd;

#if defined(DEBUG) || 1
                syslog(LOG_NOTICE, "%s(): Semaphore (id = %d) is created.", __FUNCTION__, *(ID*)ent->retvalptr);
#endif

                ercd = E_OK;
            }
		    break; }

        case TSFN_CRE_FLG: {
            syslog(LOG_DEBUG, "%s(): MOD_CFG_ENTRY TSFN_CRE_FLG", __FUNCTION__);
            assert(probe_ldm_memory(ent->argument, sizeof(T_CFLG), ldm_can));
            assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
            T_CFLG pk_cflg = *(T_CFLG*)ent->argument;
			assert(get_atrdomid(pk_cflg.flgatr) == TDOM_SELF);             // Check original DOMID
			pk_cflg.flgatr |= TA_DOM(ldm_can->domid);                      // Set new DOMID
            ercd = acre_flg(&pk_cflg);
            assert(ercd > 0);
            if(ercd > 0) {
                // Store ID
                *(ID*)ent->retvalptr = ercd;

#if defined(DEBUG) || 1
                syslog(LOG_NOTICE, "%s(): Event flag (id = %d) is created.", __FUNCTION__, *(ID*)ent->retvalptr);
#endif

                ercd = E_OK;
            }
            break; }

        case TSFN_CRE_DTQ: {
            syslog(LOG_DEBUG, "%s(): MOD_CFG_ENTRY TSFN_CRE_DTQ", __FUNCTION__);
            assert(probe_ldm_memory(ent->argument, sizeof(T_CDTQ), ldm_can));
            assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
            T_CDTQ pk_cdtq = *(T_CDTQ*)ent->argument;
			assert(pk_cdtq.dtqmb == NULL || probe_ldm_memory(pk_cdtq.dtqmb, sizeof(DTQMB) * pk_cdtq.dtqcnt, ldm_can)); // Check memory
			assert(get_atrdomid(pk_cdtq.dtqatr) == TDOM_SELF);             // Check original DOMID
			pk_cdtq.dtqatr |= TA_DOM(ldm_can->domid);                      // Set new DOMID
            ercd = acre_dtq(&pk_cdtq);
            assert(ercd > 0);
            if(ercd > 0) {
                // Store ID
                *(ID*)ent->retvalptr = ercd;

#if defined(DEBUG) || 1
                syslog(LOG_NOTICE, "%s(): Data queue (id = %d) is created.", __FUNCTION__, *(ID*)ent->retvalptr);
#endif

                ercd = E_OK;
            }
            break; }

        case TSFN_CRE_PDQ: {
            syslog(LOG_DEBUG, "%s(): MOD_CFG_ENTRY TSFN_CRE_PDQ", __FUNCTION__);
            assert(probe_ldm_memory(ent->argument, sizeof(T_CPDQ), ldm_can));
            assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
            T_CPDQ pk_cpdq = *(T_CPDQ*)ent->argument;
			assert(pk_cpdq.pdqmb == NULL); // Check memory
			assert(get_atrdomid(pk_cpdq.pdqatr) == TDOM_SELF);             // Check original DOMID
			pk_cpdq.pdqatr |= TA_DOM(ldm_can->domid);                      // Set new DOMID
            ercd = acre_pdq(&pk_cpdq);
            assert(ercd > 0);
            if(ercd > 0) {
                // Store ID
                *(ID*)ent->retvalptr = ercd;

#if defined(DEBUG) || 1
                syslog(LOG_NOTICE, "%s(): Priority data queue (id = %d) is created.", __FUNCTION__, *(ID*)ent->retvalptr);
#endif

                ercd = E_OK;
            }
            break; }

        case TSFN_CRE_MTX: {
            syslog(LOG_DEBUG, "%s(): MOD_CFG_ENTRY TSFN_CRE_MTX", __FUNCTION__);
            assert(probe_ldm_memory(ent->argument, sizeof(T_CMTX), ldm_can));
            assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
            T_CMTX pk_cmtx = *(T_CMTX*)ent->argument;
			assert(get_atrdomid(pk_cmtx.mtxatr) == TDOM_SELF);             // Check original DOMID
			pk_cmtx.mtxatr |= TA_DOM(ldm_can->domid);                      // Set new DOMID
            ercd = acre_mtx(&pk_cmtx);
            assert(ercd > 0);
            if(ercd > 0) {
                // Store ID
                *(ID*)ent->retvalptr = ercd;

#if defined(DEBUG) || 1
                syslog(LOG_NOTICE, "%s(): Mutex (id = %d) is created.", __FUNCTION__, *(ID*)ent->retvalptr);
#endif

                ercd = E_OK;
            }
            break; }

		case TSFN_CRE_CYC: {
            syslog(LOG_DEBUG, "%s(): MOD_CFG_ENTRY TSFN_CRE_CYC", __FUNCTION__);
			assert(probe_ldm_memory(ent->argument, sizeof(T_DML_CCYC), ldm_can));
			assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
            T_DML_CCYC *p_dml_ccyc = (void*)ent->argument;

            T_CCYC pk_ccyc;
            pk_ccyc.cycatr = p_dml_ccyc->cycatr;
            pk_ccyc.cyctim = p_dml_ccyc->cyctim;
            pk_ccyc.cycphs = p_dml_ccyc->cycphs;
            pk_ccyc.cycatr &= ~TA_STA;
			assert(get_atrdomid(pk_ccyc.cycatr) == TDOM_SELF);
			pk_ccyc.cycatr |= TA_DOM(ldm_can->domid);

            pk_ccyc.nfyinfo.nfymode = p_dml_ccyc->nfymode;
            switch (pk_ccyc.nfyinfo.nfymode & 0x0fU) {
            case TNFY_SETVAR:
                pk_ccyc.nfyinfo.nfy.setvar.p_var = (void*)p_dml_ccyc->par1;
                pk_ccyc.nfyinfo.nfy.setvar.value = p_dml_ccyc->par2;
                break;
            case TNFY_INCVAR:
                pk_ccyc.nfyinfo.nfy.incvar.p_var = (void*)p_dml_ccyc->par1;
                break;
            case TNFY_ACTTSK:
                pk_ccyc.nfyinfo.nfy.acttsk.tskid = *(ID*)p_dml_ccyc->par1;
                break;
            case TNFY_WUPTSK:
                pk_ccyc.nfyinfo.nfy.wuptsk.tskid = *(ID*)p_dml_ccyc->par1;
                break;
            case TNFY_SIGSEM:
                pk_ccyc.nfyinfo.nfy.sigsem.semid = *(ID*)p_dml_ccyc->par1;
                break;
            case TNFY_SETFLG:
                pk_ccyc.nfyinfo.nfy.setflg.flgid = *(ID*)p_dml_ccyc->par1;
                pk_ccyc.nfyinfo.nfy.setflg.flgptn = p_dml_ccyc->par2;
                break;
            case TNFY_SNDDTQ:
                pk_ccyc.nfyinfo.nfy.snddtq.dtqid = *(ID*)p_dml_ccyc->par1;
                pk_ccyc.nfyinfo.nfy.snddtq.data = p_dml_ccyc->par2;
                break;
            default:
                assert(false);
                break;
            }
            switch (pk_ccyc.nfyinfo.nfymode & ~0x0fU) {
            case 0:
                break;
		    case TENFY_SETVAR:
                pk_ccyc.nfyinfo.enfy.setvar.p_var = (void*)p_dml_ccyc->epar1;
			    break;
            case TENFY_INCVAR:
                pk_ccyc.nfyinfo.enfy.incvar.p_var = (void*)p_dml_ccyc->epar1;
			    break;
            case TENFY_ACTTSK:
                pk_ccyc.nfyinfo.enfy.acttsk.tskid = *(ID*)p_dml_ccyc->epar1;
                break;
            case TENFY_WUPTSK:
                pk_ccyc.nfyinfo.enfy.wuptsk.tskid = *(ID*)p_dml_ccyc->epar1;
                break;
            case TENFY_SIGSEM:
                pk_ccyc.nfyinfo.enfy.sigsem.semid = *(ID*)p_dml_ccyc->epar1;
                break;
            case TENFY_SETFLG:
                pk_ccyc.nfyinfo.enfy.setflg.flgid = *(ID*)p_dml_ccyc->epar1;
                pk_ccyc.nfyinfo.enfy.setflg.flgptn = p_dml_ccyc->epar2;
                break;
            case TENFY_SNDDTQ:
                pk_ccyc.nfyinfo.enfy.snddtq.dtqid = *(ID*)p_dml_ccyc->epar1;
                break;
            default:
                assert(false);
                break;
            }

			SVC_PERROR(ercd = acre_cyc(&pk_ccyc));
			if(ercd > 0) {
				// Store ID
			    *(ID*)ent->retvalptr = ercd;
#if defined(DEBUG) || 1
			    syslog(LOG_NOTICE, "%s(): Cyclic (id = %d) created.", __FUNCTION__, *(ID*)ent->retvalptr);
#endif

			    ercd = E_OK;
			}
			break; }

		default:
		    syslog(LOG_ERROR, "%s(): Unsupported static function code %d.", __FUNCTION__, ent->sfncd);
		    ercd = E_OBJ;
		}
	}

	// Rollback stage
	// TODO: implement this
	assert(ercd == E_OK);
	syslog(LOG_DEBUG, "%s(): text paddr: 0x%x, data paddr: 0x%x", __FUNCTION__, ldm_can->text_mempool, ldm_can->data_mempool);

	// Acting stage
    // TODO: NOTE: this stage may be too early. use HRP3 system mode to protect this?
    for(uint32_t i = 0; i < ldm_can->cfg_entry_num; ++i) {
        MOD_CFG_ENTRY *ent = &ldm_can->cfg_table[i];
        switch(ent->sfncd) {
        case TSFN_CRE_TSK: {
        	T_CTSK pk_ctsk = *(T_CTSK*)ent->argument;
        	if(pk_ctsk.tskatr & TA_ACT) {
        		ercd = act_tsk(*(ID*)ent->retvalptr);
            	assert(ercd == E_OK);
        	}
            break; }

        case TSFN_CRE_CYC: {
            T_DML_CCYC *p_dml_ccyc = (void*)ent->argument;
            if (p_dml_ccyc->cycatr & TA_STA) {
                SVC_PERROR(sta_cyc(*(ID*)ent->retvalptr));
            }
            break; }

        case TSFN_CRE_SEM:
        case TSFN_CRE_FLG:
        case TSFN_CRE_DTQ:
        case TSFN_CRE_PDQ:
        case TSFN_CRE_MTX:
            // Do nothing
            break;

        default:
            syslog(LOG_ERROR, "%s(): Unsupported static function code %d.", __FUNCTION__, ent->sfncd);
        }
    }

    return ercd;
}


ER _dmloader_ins_ldm(const uint8_t *mod_data, uint32_t mod_data_sz, ID ldm_can_id) {
	if (ldm_can_id != 1) return E_ID;
	T_LDM_CAN *ldm_can = &ldm_cans[ldm_can_id - 1];

    ER ercd;

    ercd = loc_mtx(DMLOADER_MTX);
    if(ercd != E_OK) {
        syslog(LOG_ERROR, "%s(): Acquire mutex failed.", __FUNCTION__);
        goto error_exit;
    }

    if (ldm_can->status != LDM_CAN_FREE) {
    	syslog(LOG_ERROR, "%s(): LDM container is not free.", __FUNCTION__);
    	ercd = E_OBJ;
    	goto error_exit;
    }

    elf32_ldctx_t ctx;
    ctx.data_buf = ldm_can->data_mempool;
    ctx.data_bufsz = ldm_can->data_mempool_size;
    ctx.text_buf = ldm_can->text_mempool;
    ctx.text_bufsz = ldm_can->text_mempool_size;

    /**
     * Load ELF32 data
     */
    ercd = elf32_load(mod_data, mod_data_sz, &ctx);
    if(ercd != E_OK)
        goto error_exit;
    dmloader_instruction_memory_barrier();
    uint32_t mod_pil_version = *(uint32_t*)ctx.sym__module_pil_version;
    if (PIL_VERSION != mod_pil_version) {
    	syslog(LOG_ERROR, "%s(): Wrong PIL version. FW PIL VER: %d, APP PIL VER: %d.", __FUNCTION__, PIL_VERSION, mod_pil_version);
    	ercd = E_NOSPT;
    	goto error_exit;
    }

    /**
     * Handle module configuration
     */
    ldm_can->cfg_table = ctx.sym__module_cfg_tab;
    ldm_can->cfg_entry_num = *(uint32_t*)ctx.sym__module_cfg_entry_num;
    ercd = handle_module_cfg_tab(ldm_can);
    if(ercd != E_OK)
        goto error_exit;

    ercd = unl_mtx(DMLOADER_MTX);
    assert(ercd == E_OK);

    ldm_can->status = LDM_CAN_RUNNING;
    /* Fall through */

error_exit:
    unl_mtx(DMLOADER_MTX);
    return ercd;
}

ER _dmloader_rmv_ldm(ID ldm_can_id) {
    // TODO: hard coded & should check ldm_can status
	if (ldm_can_id != 1) return E_ID;
	T_LDM_CAN *ldm_can = &ldm_cans[ldm_can_id - 1];

    ER ercd;

    ercd = loc_mtx(DMLOADER_MTX);
    if(ercd != E_OK) {
        syslog(LOG_ERROR, "%s(): Acquire mutex failed.", __FUNCTION__);
        goto error_exit;
    }

    if (ldm_can->status != LDM_CAN_RUNNING) {
    	syslog(LOG_ERROR, "%s(): LDM container is not running.", __FUNCTION__);
    	ercd = E_OBJ;
    	goto error_exit;
    }

    // Deletion
    for(uint32_t i = 0; i < ldm_can->cfg_entry_num && ercd == E_OK; ++i) {
        MOD_CFG_ENTRY *ent = &ldm_can->cfg_table[i];
        switch(ent->sfncd) {
        case TSFN_CRE_TSK: {
            syslog(LOG_DEBUG, "%s(): RMV MOD_CFG_ENTRY TSFN_CRE_TSK", __FUNCTION__);
            ID tskid = *(ID*)ent->retvalptr;
            ter_tsk(tskid);
            ercd = del_tsk(tskid);
            assert(ercd == E_OK);
            break; }

        case TSFN_CRE_SEM: {
        	syslog(LOG_DEBUG, "%s(): RMV MOD_CFG_ENTRY TSFN_CRE_SEM", __FUNCTION__);
        	ID semid = *(ID*)ent->retvalptr;
        	ercd = del_sem(semid);
        	assert(ercd == E_OK);
        	break; }

        case TSFN_CRE_FLG: {
        	syslog(LOG_DEBUG, "%s(): RMV MOD_CFG_ENTRY TSFN_CRE_FLG", __FUNCTION__);
        	ID flgid = *(ID*)ent->retvalptr;
        	ercd = del_flg(flgid);
        	assert(ercd == E_OK);
        	break; }

        case TSFN_CRE_DTQ: {
        	syslog(LOG_DEBUG, "%s(): RMV MOD_CFG_ENTRY TSFN_CRE_DTQ", __FUNCTION__);
        	ID dtqid = *(ID*)ent->retvalptr;
        	ercd = del_dtq(dtqid);
        	assert(ercd == E_OK);
        	break; }

        case TSFN_CRE_PDQ: {
        	syslog(LOG_DEBUG, "%s(): RMV MOD_CFG_ENTRY TSFN_CRE_PDQ", __FUNCTION__);
        	ID pdqid = *(ID*)ent->retvalptr;
        	ercd = del_pdq(pdqid);
        	assert(ercd == E_OK);
        	break; }

        case TSFN_CRE_MTX: {
        	syslog(LOG_DEBUG, "%s(): RMV MOD_CFG_ENTRY TSFN_CRE_MTX", __FUNCTION__);
        	ID mtxid = *(ID*)ent->retvalptr;
        	ercd = del_mtx(mtxid);
        	assert(ercd == E_OK);
        	break; }

        case TSFN_CRE_CYC: {
            syslog(LOG_DEBUG, "%s(): RMV MOD_CFG_ENTRY TSFN_CRE_CYC", __FUNCTION__);
            ID cycid = *(ID*)ent->retvalptr;
            stp_cyc(cycid);
            SVC_PERROR(ercd = del_cyc(cycid));
            break; }

        default:
            syslog(LOG_ERROR, "%s(): Unsupported static function code %d.", __FUNCTION__, ent->sfncd);
            ercd = E_OBJ;
            goto error_exit;
        }
    }

    ldm_can->status = LDM_CAN_FREE;

    // TODO: clean ldm_can
error_exit:
	unl_mtx(DMLOADER_MTX);
	return ercd;
}

ER dmloader_ref_ldm(ID ldm_can_id, T_RLDM *pk_rldm) {
    // TODO: hard coded & should check ldm_can status
	if (ldm_can_id != 1) return E_ID;
	T_LDM_CAN *ldm_can = &ldm_cans[ldm_can_id - 1];

	pk_rldm->data_mempool = ldm_can->data_mempool;
	pk_rldm->data_mempool_size = ldm_can->data_mempool_size;
	pk_rldm->text_mempool = ldm_can->text_mempool;
	pk_rldm->text_mempool_size = ldm_can->text_mempool_size;

	return E_OK;
}

/**
 * Route extended service calls to actual functions.
 */

ER_UINT extsvc_dmloader_ins_ldm(intptr_t mod_data, intptr_t mod_data_sz, intptr_t ldm_can_id, intptr_t par4, intptr_t par5, ID cdmid) {
	if (cdmid != TDOM_KERNEL) {
		syslog(LOG_ERROR, "%s(): Only firmware can call this function.", __FUNCTION__);
		return E_OACV;
	}
	return _dmloader_ins_ldm((const uint8_t *)mod_data, (uint32_t)mod_data_sz, (ID)ldm_can_id);
}

ER_UINT extsvc_dmloader_rmv_ldm(intptr_t ldm_can_id, intptr_t par2, intptr_t par3, intptr_t par4, intptr_t par5, ID cdmid) {
	if (cdmid != TDOM_KERNEL) {
		syslog(LOG_ERROR, "%s(): Only firmware can call this function.", __FUNCTION__);
		return E_OACV;
	}
	return _dmloader_rmv_ldm((ID)ldm_can_id);
}
