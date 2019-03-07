/* kernel_cfg.c */
#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#if !(TKERNEL_PRID == 0x0006U && (TKERNEL_PRVER & 0xf000U) == 0x3000U)
#error The kernel does not match this configuration file.
#endif

/*
 *  Include Directives
 */

#ifdef TOPPERS_ML_AUTO
#endif
#include "target_timer.h"
#ifdef TOPPERS_SUPPORT_OVRHDR
#endif
#include "cfg_all2.h"

/*
 *  Time Event Management
 */

TMEVTN	_kernel_tmevt_heap_kernel[1 + 1];
TMEVTN	_kernel_tmevt_heap_DOM1[1 + 20];
TMEVTN	_kernel_tmevt_heap_idle[1 + 0];

/*
 *  Task Management Functions
 */

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

static STK_T _kernel_sstack_TASK1[COUNT_STK_T(DEFAULT_SSTKSZ)] __attribute__((section(".system_stack"),nocommon));
static STK_T _kernel_ustack_TASK1[COUNT_STK_T(4096)] __attribute__((section(".ustack_TASK1"),nocommon));
static STK_T _kernel_ustack_TASK2[COUNT_STK_T(4096)] __attribute__((section(".ustack_TASK2"),nocommon));
const TINIB _kernel_tinib_table[TNUM_TSKID] = {
	{ (DOM1), (TA_ACT), (intptr_t)(1), (TASK)(task1), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(DEFAULT_SSTKSZ), _kernel_sstack_TASK1, ROUND_STK_T(4096), _kernel_ustack_TASK1, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (DOM1), (TA_NULL), (intptr_t)(2), (TASK)(task2), INT_PRIORITY(HIGH_PRIORITY), STACK_SIZE, (void *)(stack_TASK2), ROUND_STK_T(4096), _kernel_ustack_TASK2, { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

TCB _kernel_tcb_table[TNUM_TSKID];

const ID _kernel_torder_table[TNUM_TSKID] = { 
	TASK1, TASK2
};

STK_T _kernel_ustack_TOPPERS_RAM_DUMMY1[COUNT_STK_T(4096)] __attribute__((section(".ustack_TOPPERS_RAM_DUMMY1"),nocommon));
/*
 *  Semaphore Functions
 */

const ID _kernel_tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

const SEMINIB _kernel_seminib_table[TNUM_SEMID] = {
	{ (TA_NULL), (1), (1), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (TA_TPRI), (0), (2), { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

SEMCB _kernel_semcb_table[TNUM_SEMID];

/*
 *  Eventflag Functions
 */

const ID _kernel_tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

const FLGINIB _kernel_flginib_table[TNUM_FLGID] = {
	{ (TA_NULL), (0U), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (TA_TPRI), (0x0001U), { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

FLGCB _kernel_flgcb_table[TNUM_FLGID];

/*
 *  Dataqueue Functions
 */

const ID _kernel_tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

static DTQMB _kernel_dtqmb_DTQ1[10];
const DTQINIB _kernel_dtqinib_table[TNUM_DTQID] = {
	{ (TA_NULL), (10), _kernel_dtqmb_DTQ1, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (TA_TPRI), (0), NULL, { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

DTQCB _kernel_dtqcb_table[TNUM_DTQID];

/*
 *  Priority Dataqueue Functions
 */

const ID _kernel_tmax_pdqid = (TMIN_PDQID + TNUM_PDQID - 1);

static PDQMB _kernel_pdqmb_PDQ1[10];
const PDQINIB _kernel_pdqinib_table[TNUM_PDQID] = {
	{ (TA_NULL), (10), (16), _kernel_pdqmb_PDQ1, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (TA_TPRI), (0), (2), NULL, { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

PDQCB _kernel_pdqcb_table[TNUM_PDQID];

/*
 *  Mutex Functions
 */

const ID _kernel_tmax_mtxid = (TMIN_MTXID + TNUM_MTXID - 1);

const MTXINIB _kernel_mtxinib_table[TNUM_MTXID] = {
	{ (TA_NULL), INT_PRIORITY(0), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (TA_CEILING), INT_PRIORITY(MID_PRIORITY), { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

MTXCB _kernel_mtxcb_table[TNUM_MTXID];

/*
 *  Message Buffer Functions
 */

const ID _kernel_tmax_mbfid = (TMIN_MBFID + TNUM_MBFID - 1);

static MB_T _kernel_mbfmb_MBF1[TOPPERS_COUNT_SZ(TSZ_MBFMB(10,10), sizeof(MB_T))];
static MB_T _kernel_mbfmb_MBF2[TOPPERS_COUNT_SZ(TSZ_MBFMB(20,20), sizeof(MB_T))];
const MBFINIB _kernel_mbfinib_table[TNUM_MBFID] = {
	{ (TA_NULL), (10), TOPPERS_ROUND_SZ(TSZ_MBFMB(10,10), sizeof(MB_T)), _kernel_mbfmb_MBF1, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (TA_NULL), (20), TOPPERS_ROUND_SZ(TSZ_MBFMB(20,20), sizeof(MB_T)), _kernel_mbfmb_MBF2, { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

MBFCB _kernel_mbfcb_table[TNUM_MBFID];

/*
 *  Fixed-sized Memorypool Functions
 */

const ID _kernel_tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

static MPF_T _kernel_mpf_MPF1[(10) * COUNT_MPF_T(100)] __attribute__((section(".mpf_MPF1"),nocommon));
static MPFMB _kernel_mpfmb_MPF1[10];
static MPFMB _kernel_mpfmb_MPF2[20];
const MPFINIB _kernel_mpfinib_table[TNUM_MPFID] = {
	{ (TA_NULL), (10), ROUND_MPF_T(100), _kernel_mpf_MPF1, _kernel_mpfmb_MPF1, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ (TA_NULL), (20), ROUND_MPF_T(200), (void *)(mpf_MPF2), _kernel_mpfmb_MPF2, { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

MPFCB _kernel_mpfcb_table[TNUM_MPFID];

/*
 *  Cyclic Notification Functions
 */

const ID _kernel_tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);

static void
_kernel_nfyhdr_CYC2(intptr_t exinf)
{
	*((intptr_t *) exinf) = (1);
}

static void
_kernel_nfyhdr_CYC3(intptr_t exinf)
{
	(void) loc_cpu();
	*((intptr_t *) exinf) += 1;
	(void) unl_cpu();
}

const ID _kernel_nfyhdr_CYC4_etskid = TASK2;

static void
_kernel_nfyhdr_CYC4(intptr_t exinf)
{
	ER	ercd;

	ercd = act_tsk((ID) exinf);
	if (ercd != E_OK) {
		(void) act_tsk(_kernel_nfyhdr_CYC4_etskid);
	}
}

const ID _kernel_nfyhdr_CYC5_etskid = TASK2;

static void
_kernel_nfyhdr_CYC5(intptr_t exinf)
{
	ER	ercd;

	ercd = wup_tsk((ID) exinf);
	if (ercd != E_OK) {
		(void) wup_tsk(_kernel_nfyhdr_CYC5_etskid);
	}
}

const ID _kernel_nfyhdr_CYC6_esemid = SEM2;

static void
_kernel_nfyhdr_CYC6(intptr_t exinf)
{
	ER	ercd;

	ercd = sig_sem((ID) exinf);
	if (ercd != E_OK) {
		(void) sig_sem(_kernel_nfyhdr_CYC6_esemid);
	}
}

const ID _kernel_nfyhdr_CYC7_eflgid = FLG1;

static void
_kernel_nfyhdr_CYC7(intptr_t exinf)
{
	ER	ercd;

	ercd = set_flg(((ID) exinf), 0x01);
	if (ercd != E_OK) {
		(void) set_flg(_kernel_nfyhdr_CYC7_eflgid, 0x02);
	}
}

const ID _kernel_nfyhdr_CYC8_edtqid = DTQ2;

static void
_kernel_nfyhdr_CYC8(intptr_t exinf)
{
	ER	ercd;

	ercd = psnd_dtq(((ID) exinf), 0x00);
	if (ercd != E_OK) {
		(void) psnd_dtq(_kernel_nfyhdr_CYC8_edtqid, (intptr_t) ercd);
	}
}

intptr_t *const _kernel_nfyhdr_CYC9_p_evar = (intptr_t *)(&var1);

static void
_kernel_nfyhdr_CYC9(intptr_t exinf)
{
	ER	ercd;

	ercd = act_tsk((ID) exinf);
	if (ercd != E_OK) {
		*_kernel_nfyhdr_CYC9_p_evar = (intptr_t) ercd;
	}
}

intptr_t *const _kernel_nfyhdr_CYC10_p_evar = (intptr_t *)(&var2);

static void
_kernel_nfyhdr_CYC10(intptr_t exinf)
{
	ER	ercd;

	ercd = act_tsk((ID) exinf);
	if (ercd != E_OK) {
		(void) loc_cpu();
		*_kernel_nfyhdr_CYC10_p_evar += 1;
		(void) unl_cpu();
	}
}

const CYCINIB _kernel_cycinib_table[TNUM_CYCID] = {
	{ _kernel_tmevt_heap_DOM1, (TA_STA), (intptr_t)(&var1), _kernel_nfyhdr_CYC2, (1000), (1000), { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(&var2), _kernel_nfyhdr_CYC3, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC4, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC5, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(SEM1), _kernel_nfyhdr_CYC6, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(FLG1), _kernel_nfyhdr_CYC7, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(DTQ1), _kernel_nfyhdr_CYC8, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC9, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC10, (1000), (1000), { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } }
};

CYCCB _kernel_cyccb_table[TNUM_CYCID];

/*
 *  Alarm Notification Functions
 */

const ID _kernel_tmax_almid = (TMIN_ALMID + TNUM_ALMID - 1);

static void
_kernel_nfyhdr_ALM2(intptr_t exinf)
{
	*((intptr_t *) exinf) = (1);
}

static void
_kernel_nfyhdr_ALM3(intptr_t exinf)
{
	(void) loc_cpu();
	*((intptr_t *) exinf) += 1;
	(void) unl_cpu();
}

const ID _kernel_nfyhdr_ALM4_etskid = TASK2;

static void
_kernel_nfyhdr_ALM4(intptr_t exinf)
{
	ER	ercd;

	ercd = act_tsk((ID) exinf);
	if (ercd != E_OK) {
		(void) act_tsk(_kernel_nfyhdr_ALM4_etskid);
	}
}

const ID _kernel_nfyhdr_ALM5_etskid = TASK2;

static void
_kernel_nfyhdr_ALM5(intptr_t exinf)
{
	ER	ercd;

	ercd = wup_tsk((ID) exinf);
	if (ercd != E_OK) {
		(void) wup_tsk(_kernel_nfyhdr_ALM5_etskid);
	}
}

const ID _kernel_nfyhdr_ALM6_esemid = SEM2;

static void
_kernel_nfyhdr_ALM6(intptr_t exinf)
{
	ER	ercd;

	ercd = sig_sem((ID) exinf);
	if (ercd != E_OK) {
		(void) sig_sem(_kernel_nfyhdr_ALM6_esemid);
	}
}

const ID _kernel_nfyhdr_ALM7_eflgid = FLG1;

static void
_kernel_nfyhdr_ALM7(intptr_t exinf)
{
	ER	ercd;

	ercd = set_flg(((ID) exinf), 0x01);
	if (ercd != E_OK) {
		(void) set_flg(_kernel_nfyhdr_ALM7_eflgid, 0x02);
	}
}

const ID _kernel_nfyhdr_ALM8_edtqid = DTQ2;

static void
_kernel_nfyhdr_ALM8(intptr_t exinf)
{
	ER	ercd;

	ercd = psnd_dtq(((ID) exinf), 0x00);
	if (ercd != E_OK) {
		(void) psnd_dtq(_kernel_nfyhdr_ALM8_edtqid, (intptr_t) ercd);
	}
}

intptr_t *const _kernel_nfyhdr_ALM9_p_evar = (intptr_t *)(&var1);

static void
_kernel_nfyhdr_ALM9(intptr_t exinf)
{
	ER	ercd;

	ercd = act_tsk((ID) exinf);
	if (ercd != E_OK) {
		*_kernel_nfyhdr_ALM9_p_evar = (intptr_t) ercd;
	}
}

intptr_t *const _kernel_nfyhdr_ALM10_p_evar = (intptr_t *)(&var2);

static void
_kernel_nfyhdr_ALM10(intptr_t exinf)
{
	ER	ercd;

	ercd = act_tsk((ID) exinf);
	if (ercd != E_OK) {
		(void) loc_cpu();
		*_kernel_nfyhdr_ALM10_p_evar += 1;
		(void) unl_cpu();
	}
}

const ALMINIB _kernel_alminib_table[TNUM_ALMID] = {
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(&var1), _kernel_nfyhdr_ALM2, { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(&var2), _kernel_nfyhdr_ALM3, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM4, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM5, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(SEM1), _kernel_nfyhdr_ALM6, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(FLG1), _kernel_nfyhdr_ALM7, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(DTQ1), _kernel_nfyhdr_ALM8, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM9, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } },
	{ _kernel_tmevt_heap_DOM1, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM10, { TACP(DOM1), TACP(DOM1), TACP_SHARED, TACP(DOM1) } }
};

ALMCB _kernel_almcb_table[TNUM_ALMID];

/*
 *  Interrupt Management Functions
 */

#ifndef LOG_ISR_ENTER
#define LOG_ISR_ENTER(isrid)
#endif /* LOG_ISR_ENTER */

#ifndef LOG_ISR_LEAVE
#define LOG_ISR_LEAVE(isrid)
#endif /* LOG_ISR_LEAVE */

#define TNUM_DEF_INHNO	2
const uint_t _kernel_tnum_def_inhno = TNUM_DEF_INHNO;

INTHDR_ENTRY(INHNO_HRT, 0, _kernel_target_hrt_handler)
INTHDR_ENTRY(INHNO_TWDTIMER, 1, _kernel_target_twdtimer_handler)

const INHINIB _kernel_inhinib_table[TNUM_DEF_INHNO] = {
	{ (INHNO_HRT), (TA_NULL), (FP)(INT_ENTRY(INHNO_HRT, _kernel_target_hrt_handler)) },
	{ (INHNO_TWDTIMER), (TA_NULL), (FP)(INT_ENTRY(INHNO_TWDTIMER, _kernel_target_twdtimer_handler)) }
};

#define TNUM_CFG_INTNO	2
const uint_t _kernel_tnum_cfg_intno = TNUM_CFG_INTNO;

const INTINIB _kernel_intinib_table[TNUM_CFG_INTNO] = {
	{ (INTNO_HRT), (TA_ENAINT | INTATR_HRT), (INTPRI_HRT) },
	{ (INTNO_TWDTIMER), (TA_ENAINT|INTATR_TWDTIMER), (INTPRI_TWDTIMER) }
};

/*
 *  CPU Exception Management Functions
 */

#define TNUM_DEF_EXCNO	0
const uint_t _kernel_tnum_def_excno = TNUM_DEF_EXCNO;

TOPPERS_EMPTY_LABEL(const EXCINIB, _kernel_excinib_table);

/*
 *  Extended Service Calls
 */

const FN _kernel_tmax_fncd = TMAX_FNCD;

TOPPERS_EMPTY_LABEL(const SVCINIB, _kernel_svcinib_table);

/*
 *  Access Permission Vector
 */

const ACVCT _kernel_sysstat_acvct = { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL };

/*
 *  Stack Area for Non-task Context
 */

static STK_T _kernel_istack[COUNT_STK_T(DEFAULT_ISTKSZ)] __attribute__((section(".system_stack"),nocommon));
const size_t _kernel_istksz = ROUND_STK_T(DEFAULT_ISTKSZ);
STK_T *const _kernel_istk = _kernel_istack;

#ifdef TOPPERS_ISTKPT
STK_T *const _kernel_istkpt = TOPPERS_ISTKPT(_kernel_istack, ROUND_STK_T(DEFAULT_ISTKSZ));
#endif /* TOPPERS_ISTKPT */

/*
 *  Module Initialization Function
 */

void
_kernel_initialize_object(void)
{
	_kernel_initialize_task();
	_kernel_initialize_semaphore();
	_kernel_initialize_eventflag();
	_kernel_initialize_dataqueue();
	_kernel_initialize_pridataq();
	_kernel_initialize_mutex();
	_kernel_initialize_messagebuf();
	_kernel_initialize_mempfix();
	_kernel_initialize_cyclic();
	_kernel_initialize_alarm();
	_kernel_initialize_interrupt();
	_kernel_initialize_exception();
}

/*
 *  Initialization Routine
 */

const uint_t _kernel_tnum_inirtn = TNUM_INIRTN;

const INIRTNB _kernel_inirtnb_table[TNUM_INIRTN] = {
	{ (INIRTN)(_kernel_target_hrt_initialize), (intptr_t)(0) },
	{ (INIRTN)(_kernel_target_twdtimer_initialize), (intptr_t)(0) }
};

/*
 *  Termination Routine
 */

const uint_t _kernel_tnum_terrtn = TNUM_TERRTN;

const TERRTNB _kernel_terrtnb_table[TNUM_TERRTN] = {
	{ (TERRTN)(_kernel_target_twdtimer_terminate), (intptr_t)(0) },
	{ (TERRTN)(_kernel_target_hrt_terminate), (intptr_t)(0) }
};

/*
 *  Temporal Partitioning Functions
 */

const RELTIM _kernel_system_cyctim = 10000;

const uint_t _kernel_tnum_schedcb = 1;

SCHEDCB _kernel_schedcb_table[1];

const TWDINIB _kernel_twdinib_table[4] = {
	{ 4000, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), (intptr_t)(0), NULL },
	{ 4000, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), (intptr_t)(0), NULL },
	{ 5000, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), (intptr_t)(0), NULL },
	{ 3000, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), (intptr_t)(0), NULL }
};

const ID _kernel_tmax_somid = (TMIN_SOMID + TNUM_SOMID - 1);

const SOMINIB *const _kernel_p_inisom = &(_kernel_sominib_table[INDEX_SOM(SOM1)]);

const SOMINIB _kernel_sominib_table[TNUM_SOMID + 1] = {
	{ &(_kernel_twdinib_table[0]), &(_kernel_sominib_table[INDEX_SOM(SOM1)]) },
	{ &(_kernel_twdinib_table[2]), &(_kernel_sominib_table[INDEX_SOM(SOM1)]) },
	{ &(_kernel_twdinib_table[4]), NULL }
};

/*
 *  Protection Domain Management Functions
 */

const ID _kernel_tmax_domid = (TMIN_DOMID + TNUM_DOMID - 1);

const DOMINIB _kernel_dominib_kernel = { TACP_KERNEL, &(_kernel_schedcb_kernel), _kernel_tmevt_heap_kernel, INT_PRIORITY(TMIN_TPRI + 1), { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL } };

const DOMINIB _kernel_dominib_table[TNUM_DOMID] = {
	{ TACP(DOM1), &(_kernel_schedcb_table[0]), _kernel_tmevt_heap_DOM1, INT_PRIORITY(3), { TACP_SHARED, TACP(DOM1), TACP(DOM1), TACP_SHARED } }
};

