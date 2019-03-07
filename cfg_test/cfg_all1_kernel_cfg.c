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
#include "cfg_all1.h"

/*
 *  Time Event Management
 */

TMEVTN	_kernel_tmevt_heap_kernel[1 + TNUM_TSKID + TNUM_CYCID + TNUM_ALMID];
TOPPERS_EMPTY_LABEL(TMEVTN, _kernel_tmevt_heap_idle);

/*
 *  Task Management Functions
 */

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

static STK_T _kernel_sstack_TASK1[COUNT_STK_T(STACK_SIZE)] __attribute__((section(".system_stack"),nocommon));
const TINIB _kernel_tinib_table[TNUM_TSKID] = {
	{ (TDOM_KERNEL), (TA_ACT), (intptr_t)(1), (TASK)(task1), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(STACK_SIZE), _kernel_sstack_TASK1, 0, NULL, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TDOM_KERNEL), (TA_NULL), (intptr_t)(2), (TASK)(task2), INT_PRIORITY(HIGH_PRIORITY), STACK_SIZE, (void *)(stack_TASK2), 0, NULL, { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
};

TCB _kernel_tcb_table[TNUM_TSKID];

const ID _kernel_torder_table[TNUM_TSKID] = { 
	TASK1, TASK2
};

/*
 *  Semaphore Functions
 */

const ID _kernel_tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

const SEMINIB _kernel_seminib_table[TNUM_SEMID] = {
	{ (TA_NULL), (1), (1), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TA_TPRI), (0), (2), { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
};

SEMCB _kernel_semcb_table[TNUM_SEMID];

/*
 *  Eventflag Functions
 */

const ID _kernel_tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

const FLGINIB _kernel_flginib_table[TNUM_FLGID] = {
	{ (TA_NULL), (0U), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TA_TPRI), (0x0001U), { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
};

FLGCB _kernel_flgcb_table[TNUM_FLGID];

/*
 *  Dataqueue Functions
 */

const ID _kernel_tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

static DTQMB _kernel_dtqmb_DTQ1[10];
const DTQINIB _kernel_dtqinib_table[TNUM_DTQID] = {
	{ (TA_NULL), (10), _kernel_dtqmb_DTQ1, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TA_TPRI), (0), NULL, { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
};

DTQCB _kernel_dtqcb_table[TNUM_DTQID];

/*
 *  Priority Dataqueue Functions
 */

const ID _kernel_tmax_pdqid = (TMIN_PDQID + TNUM_PDQID - 1);

static PDQMB _kernel_pdqmb_PDQ1[10];
const PDQINIB _kernel_pdqinib_table[TNUM_PDQID] = {
	{ (TA_NULL), (10), (16), _kernel_pdqmb_PDQ1, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TA_TPRI), (0), (2), NULL, { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
};

PDQCB _kernel_pdqcb_table[TNUM_PDQID];

/*
 *  Mutex Functions
 */

const ID _kernel_tmax_mtxid = (TMIN_MTXID + TNUM_MTXID - 1);

const MTXINIB _kernel_mtxinib_table[TNUM_MTXID] = {
	{ (TA_NULL), INT_PRIORITY(0), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TA_CEILING), INT_PRIORITY(MID_PRIORITY), { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
};

MTXCB _kernel_mtxcb_table[TNUM_MTXID];

/*
 *  Message Buffer Functions
 */

const ID _kernel_tmax_mbfid = (TMIN_MBFID + TNUM_MBFID - 1);

static MB_T _kernel_mbfmb_MBF1[TOPPERS_COUNT_SZ(TSZ_MBFMB(10,10), sizeof(MB_T))];
static MB_T _kernel_mbfmb_MBF2[TOPPERS_COUNT_SZ(TSZ_MBFMB(20,20), sizeof(MB_T))];
const MBFINIB _kernel_mbfinib_table[TNUM_MBFID] = {
	{ (TA_NULL), (10), TOPPERS_ROUND_SZ(TSZ_MBFMB(10,10), sizeof(MB_T)), _kernel_mbfmb_MBF1, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TA_NULL), (20), TOPPERS_ROUND_SZ(TSZ_MBFMB(20,20), sizeof(MB_T)), _kernel_mbfmb_MBF2, { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
};

MBFCB _kernel_mbfcb_table[TNUM_MBFID];

/*
 *  Fixed-sized Memorypool Functions
 */

const ID _kernel_tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

static MPF_T _kernel_mpf_MPF1[10 * COUNT_MPF_T(100)];
static MPFMB _kernel_mpfmb_MPF1[10];
static MPFMB _kernel_mpfmb_MPF2[20];
const MPFINIB _kernel_mpfinib_table[TNUM_MPFID] = {
	{ (TA_NULL), (10), ROUND_MPF_T(100), _kernel_mpf_MPF1, _kernel_mpfmb_MPF1, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ (TA_NULL), (20), ROUND_MPF_T(200), (void *)(mpf_MPF2), _kernel_mpfmb_MPF2, { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } }
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
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(1), (NFYHDR)(cyclic1_handler), (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_STA), (intptr_t)(&var1), _kernel_nfyhdr_CYC2, (1000), (1000), { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(&var2), _kernel_nfyhdr_CYC3, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC4, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC5, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(SEM1), _kernel_nfyhdr_CYC6, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(FLG1), _kernel_nfyhdr_CYC7, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(DTQ1), _kernel_nfyhdr_CYC8, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC9, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_CYC10, (1000), (1000), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } }
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
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(1), (NFYHDR)(alarm1_handler), { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(&var1), _kernel_nfyhdr_ALM2, { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(&var2), _kernel_nfyhdr_ALM3, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM4, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM5, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(SEM1), _kernel_nfyhdr_ALM6, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(FLG1), _kernel_nfyhdr_ALM7, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(DTQ1), _kernel_nfyhdr_ALM8, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM9, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } },
	{ _kernel_tmevt_heap_kernel, (TA_NULL), (intptr_t)(TASK1), _kernel_nfyhdr_ALM10, { TACP_KERNEL, TACP_KERNEL, TACP_SHARED, TACP_KERNEL } }
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

void
_kernel_inthdr_5(void)
{
	LOG_ISR_ENTER(ISR3);
	((ISR)(isr3))((intptr_t)(3));
	LOG_ISR_LEAVE(ISR3);

	if (sense_lock()) {
		unlock_cpu();
	}

	LOG_ISR_ENTER(ISR1);
	((ISR)(isr1))((intptr_t)(1));
	LOG_ISR_LEAVE(ISR1);

	if (sense_lock()) {
		unlock_cpu();
	}

	LOG_ISR_ENTER(ISR2);
	((ISR)(isr2))((intptr_t)(2));
	LOG_ISR_LEAVE(ISR2);
}

#define TNUM_DEF_INHNO	4
const uint_t _kernel_tnum_def_inhno = TNUM_DEF_INHNO;

INTHDR_ENTRY(INHNO_HRT, 0, _kernel_target_hrt_handler)
INTHDR_ENTRY(INHNO_TWDTIMER, 1, _kernel_target_twdtimer_handler)
INTHDR_ENTRY(INHNO1, 4, int1_handler)
INTHDR_ENTRY(5, 5, _kernel_inthdr_5)

const INHINIB _kernel_inhinib_table[TNUM_DEF_INHNO] = {
	{ (INHNO_HRT), (TA_NULL), (FP)(INT_ENTRY(INHNO_HRT, _kernel_target_hrt_handler)) },
	{ (INHNO_TWDTIMER), (TA_NULL), (FP)(INT_ENTRY(INHNO_TWDTIMER, _kernel_target_twdtimer_handler)) },
	{ (INHNO1), (TA_NULL), (FP)(INT_ENTRY(INHNO1, int1_handler)) },
	{ (5), (TA_NULL), (FP)(INT_ENTRY(5, _kernel_inthdr_5)) }
};

#define TNUM_CFG_INTNO	5
const uint_t _kernel_tnum_cfg_intno = TNUM_CFG_INTNO;

const INTINIB _kernel_intinib_table[TNUM_CFG_INTNO] = {
	{ (INTNO_HRT), (TA_ENAINT | INTATR_HRT), (INTPRI_HRT) },
	{ (INTNO_TWDTIMER), (TA_ENAINT|INTATR_TWDTIMER), (INTPRI_TWDTIMER) },
	{ (INTNO1), (TA_NULL), (MAX_INT_PRIORITY) },
	{ (INTNO2), (TA_ENAINT), (MIN_INT_PRIORITY) },
	{ (INTNO3), (TA_NULL), (MAX_INT_PRIORITY) }
};

/*
 *  CPU Exception Management Functions
 */

#define TNUM_DEF_EXCNO	2
const uint_t _kernel_tnum_def_excno = TNUM_DEF_EXCNO;

EXCHDR_ENTRY(CPUEXC1, 1, cpuexc1_handler)
EXCHDR_ENTRY(CPUEXC2, 2, cpuexc2_handler)

const EXCINIB _kernel_excinib_table[TNUM_DEF_EXCNO] = {
	{ (CPUEXC1), (TA_NULL), (FP)(EXC_ENTRY(CPUEXC1, cpuexc1_handler)) },
	{ (CPUEXC2), (TA_NULL), (FP)(EXC_ENTRY(CPUEXC2, cpuexc2_handler)) }
};

/*
 *  Extended Service Calls
 */

const FN _kernel_tmax_fncd = TMAX_FNCD;

const SVCINIB _kernel_svcinib_table[TMAX_FNCD] = {
	{ (EXTSVC)(extended_svc1), 100 },
	{ NULL, 0 },
	{ (EXTSVC)(extended_svc3), 100 }
};

/*
 *  Access Permission Vector
 */

const ACVCT _kernel_sysstat_acvct = { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED };

/*
 *  Stack Area for Non-task Context
 */

const size_t _kernel_istksz = (10000);
STK_T *const _kernel_istk = (void *)(istack);

#ifdef TOPPERS_ISTKPT
STK_T *const _kernel_istkpt = TOPPERS_ISTKPT((void *)(istack), (10000));
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
	{ (INIRTN)(_kernel_target_twdtimer_initialize), (intptr_t)(0) },
	{ (INIRTN)(initialize1_routine), (intptr_t)(1) },
	{ (INIRTN)(initialize2_routine), (intptr_t)(2) }
};

/*
 *  Termination Routine
 */

const uint_t _kernel_tnum_terrtn = TNUM_TERRTN;

const TERRTNB _kernel_terrtnb_table[TNUM_TERRTN] = {
	{ (TERRTN)(terminate2_routine), (intptr_t)(2) },
	{ (TERRTN)(terminate1_routine), (intptr_t)(1) },
	{ (TERRTN)(_kernel_target_twdtimer_terminate), (intptr_t)(0) },
	{ (TERRTN)(_kernel_target_hrt_terminate), (intptr_t)(0) }
};

/*
 *  Temporal Partitioning Functions
 */

const RELTIM _kernel_system_cyctim = 0U;
const uint_t _kernel_tnum_schedcb = 0U;
TOPPERS_EMPTY_LABEL(SCHEDCB, _kernel_schedcb_table);
TOPPERS_EMPTY_LABEL(const TWDINIB, _kernel_twdinib_table);
const ID _kernel_tmax_somid = 0;
const SOMINIB *const _kernel_p_inisom = NULL;
TOPPERS_EMPTY_LABEL(const SOMINIB, _kernel_sominib_table);

/*
 *  Protection Domain Management Functions
 */

const ID _kernel_tmax_domid = (TMIN_DOMID + TNUM_DOMID - 1);

const DOMINIB _kernel_dominib_kernel = { TACP_KERNEL, &(_kernel_schedcb_kernel), _kernel_tmevt_heap_kernel, INT_PRIORITY(3), { TACP_SHARED, TACP_KERNEL, TACP_KERNEL, TACP_SHARED } };

TOPPERS_EMPTY_LABEL(const DOMINIB, _kernel_dominib_table);

