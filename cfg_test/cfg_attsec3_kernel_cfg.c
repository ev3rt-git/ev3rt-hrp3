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
#include "cfg_attsec.h"

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
	{ (TDOM_KERNEL), (TA_ACT), (intptr_t)(1), (TASK)(task1), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(STACK_SIZE), _kernel_sstack_TASK1, 0, NULL, { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL } }
};

TCB _kernel_tcb_table[TNUM_TSKID];

const ID _kernel_torder_table[TNUM_TSKID] = { 
	TASK1
};

/*
 *  Semaphore Functions
 */

const ID _kernel_tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

TOPPERS_EMPTY_LABEL(const SEMINIB, _kernel_seminib_table);
TOPPERS_EMPTY_LABEL(SEMCB, _kernel_semcb_table);

/*
 *  Eventflag Functions
 */

const ID _kernel_tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

TOPPERS_EMPTY_LABEL(const FLGINIB, _kernel_flginib_table);
TOPPERS_EMPTY_LABEL(FLGCB, _kernel_flgcb_table);

/*
 *  Dataqueue Functions
 */

const ID _kernel_tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

TOPPERS_EMPTY_LABEL(const DTQINIB, _kernel_dtqinib_table);
TOPPERS_EMPTY_LABEL(DTQCB, _kernel_dtqcb_table);

/*
 *  Priority Dataqueue Functions
 */

const ID _kernel_tmax_pdqid = (TMIN_PDQID + TNUM_PDQID - 1);

TOPPERS_EMPTY_LABEL(const PDQINIB, _kernel_pdqinib_table);
TOPPERS_EMPTY_LABEL(PDQCB, _kernel_pdqcb_table);

/*
 *  Mutex Functions
 */

const ID _kernel_tmax_mtxid = (TMIN_MTXID + TNUM_MTXID - 1);

TOPPERS_EMPTY_LABEL(const MTXINIB, _kernel_mtxinib_table);
TOPPERS_EMPTY_LABEL(MTXCB, _kernel_mtxcb_table);

/*
 *  Message Buffer Functions
 */

const ID _kernel_tmax_mbfid = (TMIN_MBFID + TNUM_MBFID - 1);

TOPPERS_EMPTY_LABEL(const MBFINIB, _kernel_mbfinib_table);
TOPPERS_EMPTY_LABEL(MBFCB, _kernel_mbfcb_table);

/*
 *  Fixed-sized Memorypool Functions
 */

const ID _kernel_tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

TOPPERS_EMPTY_LABEL(const MPFINIB, _kernel_mpfinib_table);
TOPPERS_EMPTY_LABEL(MPFCB, _kernel_mpfcb_table);

/*
 *  Cyclic Notification Functions
 */

const ID _kernel_tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);

TOPPERS_EMPTY_LABEL(const CYCINIB, _kernel_cycinib_table);
TOPPERS_EMPTY_LABEL(CYCCB, _kernel_cyccb_table);

/*
 *  Alarm Notification Functions
 */

const ID _kernel_tmax_almid = (TMIN_ALMID + TNUM_ALMID - 1);

TOPPERS_EMPTY_LABEL(const ALMINIB, _kernel_alminib_table);
TOPPERS_EMPTY_LABEL(ALMCB, _kernel_almcb_table);

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

const DOMINIB _kernel_dominib_kernel = { TACP_KERNEL, &(_kernel_schedcb_kernel), _kernel_tmevt_heap_kernel, INT_PRIORITY(TMIN_TPRI + 1), { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL } };

TOPPERS_EMPTY_LABEL(const DOMINIB, _kernel_dominib_table);

