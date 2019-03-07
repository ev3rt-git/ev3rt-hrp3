/* This file is generated from core_rename.def by genrename. */

#ifndef TOPPERS_CORE_RENAME_H
#define TOPPERS_CORE_RENAME_H

/*
 *  kernel_cfg.c
 */
#define inh_table					_kernel_inh_table
#define intcfg_table				_kernel_intcfg_table
#define exc_table					_kernel_exc_table

/*
 *  kernel_mem.c
 */
#define section_table				_kernel_section_table
#define page_table					_kernel_page_table

/*
 *  core_support.S
 */
#define dispatch					_kernel_dispatch
#define start_dispatch				_kernel_start_dispatch
#define exit_and_dispatch			_kernel_exit_and_dispatch
#define call_exit_kernel			_kernel_call_exit_kernel
#define start_stask_r				_kernel_start_stask_r
#define start_utask_r				_kernel_start_utask_r
#define irq_handler					_kernel_irq_handler
#define undef_handler				_kernel_undef_handler
#define svc_handler					_kernel_svc_handler
#define pabort_handler				_kernel_pabort_handler
#define dabort_handler				_kernel_dabort_handler
#define fiq_handler					_kernel_fiq_handler
#define scycovr_handler				_kernel_scycovr_handler

/*
 *  core_kernel_impl.c
 */
#define excpt_nest_count			_kernel_excpt_nest_count
#define arm_mmu_initialize			_kernel_arm_mmu_initialize
#define arm_fpu_initialize			_kernel_arm_fpu_initialize
#define core_initialize				_kernel_core_initialize
#define core_terminate				_kernel_core_terminate
#define call_ext_tsk				_kernel_call_ext_tsk
#define xlog_sys					_kernel_xlog_sys
#define xlog_fsr					_kernel_xlog_fsr
#define default_int_handler			_kernel_default_int_handler
#define default_exc_handler			_kernel_default_exc_handler

/*
 *  mpcore_kernel_impl.c
 */
#define mpcore_initialize			_kernel_mpcore_initialize
#define mpcore_terminate			_kernel_mpcore_terminate

/*
 *  mpcore_timer.c
 */
#define target_hrt_initialize		_kernel_target_hrt_initialize
#define target_hrt_terminate		_kernel_target_hrt_terminate
#define target_hrt_handler			_kernel_target_hrt_handler
#define target_twdtimer_initialize	_kernel_target_twdtimer_initialize
#define target_twdtimer_terminate	_kernel_target_twdtimer_terminate
#define target_twdtimer_handler		_kernel_target_twdtimer_handler
#define target_ovrtimer_initialize	_kernel_target_ovrtimer_initialize
#define target_ovrtimer_terminate	_kernel_target_ovrtimer_terminate
#define target_ovrtimer_handler		_kernel_target_ovrtimer_handler

/*
 *  gic_kernel_impl.c
 */
#define gicc_initialize				_kernel_gicc_initialize
#define gicc_terminate				_kernel_gicc_terminate
#define gicd_initialize				_kernel_gicd_initialize
#define gicd_terminate				_kernel_gicd_terminate

/*
 *  gic_support.S
 */
#define irc_begin_int				_kernel_irc_begin_int
#define irc_end_int					_kernel_irc_end_int
#define irc_get_intpri				_kernel_irc_get_intpri
#define irc_begin_exc				_kernel_irc_begin_exc
#define irc_end_exc					_kernel_irc_end_exc

/*
 *  pl310.c
 */
#define pl310_initialize			_kernel_pl310_initialize
#define pl310_disable				_kernel_pl310_disable
#define pl310_invalidate_all		_kernel_pl310_invalidate_all
#define pl310_clean_and_invalidate_all	_kernel_pl310_clean_and_invalidate_all


#endif /* TOPPERS_CORE_RENAME_H */
