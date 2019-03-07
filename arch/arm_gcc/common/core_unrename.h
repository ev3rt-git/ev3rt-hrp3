/* This file is generated from core_rename.def by genrename. */

/* This file is included only when core_rename.h has been included. */
#ifdef TOPPERS_CORE_RENAME_H
#undef TOPPERS_CORE_RENAME_H

/*
 *  kernel_cfg.c
 */
#undef inh_table
#undef intcfg_table
#undef exc_table

/*
 *  kernel_mem.c
 */
#undef section_table
#undef page_table

/*
 *  core_support.S
 */
#undef dispatch
#undef start_dispatch
#undef exit_and_dispatch
#undef call_exit_kernel
#undef start_stask_r
#undef start_utask_r
#undef irq_handler
#undef undef_handler
#undef svc_handler
#undef pabort_handler
#undef dabort_handler
#undef fiq_handler
#undef scycovr_handler

/*
 *  core_kernel_impl.c
 */
#undef excpt_nest_count
#undef arm_mmu_initialize
#undef arm_fpu_initialize
#undef core_initialize
#undef core_terminate
#undef call_ext_tsk
#undef xlog_sys
#undef xlog_fsr
#undef default_int_handler
#undef default_exc_handler

/*
 *  mpcore_kernel_impl.c
 */
#undef mpcore_initialize
#undef mpcore_terminate

/*
 *  mpcore_timer.c
 */
#undef target_hrt_initialize
#undef target_hrt_terminate
#undef target_hrt_handler
#undef target_twdtimer_initialize
#undef target_twdtimer_terminate
#undef target_twdtimer_handler
#undef target_ovrtimer_initialize
#undef target_ovrtimer_terminate
#undef target_ovrtimer_handler

/*
 *  gic_kernel_impl.c
 */
#undef gicc_initialize
#undef gicc_terminate
#undef gicd_initialize
#undef gicd_terminate

/*
 *  gic_support.S
 */
#undef irc_begin_int
#undef irc_end_int
#undef irc_get_intpri
#undef irc_begin_exc
#undef irc_end_exc

/*
 *  pl310.c
 */
#undef pl310_initialize
#undef pl310_disable
#undef pl310_invalidate_all
#undef pl310_clean_and_invalidate_all


#endif /* TOPPERS_CORE_RENAME_H */
