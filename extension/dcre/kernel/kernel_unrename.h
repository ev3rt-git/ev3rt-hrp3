/* This file is generated from kernel_rename.def by genrename. */

/* This file is included only when kernel_rename.h has been included. */
#ifdef TOPPERS_KERNEL_RENAME_H
#undef TOPPERS_KERNEL_RENAME_H

/*
 *  startup.c
 */
#undef kerflg
#undef exit_kernel
#undef initialize_mempool
#undef malloc_mempool
#undef free_mempool
#undef malloc_mpk
#undef free_mpk

/*
 *  domain.c
 */
#undef p_twdsched
#undef p_idlesched
#undef schedcb_kernel
#undef schedcb_idle
#undef p_cursom
#undef p_nxtsom
#undef scyc_tmevtb
#undef pending_scycswitch
#undef p_runtwd
#undef pending_twdswitch
#undef twdtimer_enable
#undef twdtimer_flag
#undef left_twdtim
#undef rundom
#undef p_ctxdom
#undef initialize_domain
#undef twdtimer_start
#undef twdtimer_stop
#undef twdtimer_control
#undef scyc_start
#undef scyc_switch
#undef twd_start
#undef twd_switch
#undef set_dspflg

/*
 *  task.c
 */
#undef p_runtsk
#undef p_schedtsk
#undef enadsp
#undef dspflg
#undef initialize_task
#undef search_predtsk
#undef make_runnable
#undef make_non_runnable
#undef make_dormant
#undef make_active
#undef change_priority
#undef rotate_ready_queue
#undef task_terminate

/*
 *  taskhook.c
 */
#undef mtxhook_check_ceilpri
#undef mtxhook_scan_ceilmtx
#undef mtxhook_release_all
#undef mbfhook_dequeue_wobj

/*
 *  wait.c
 */
#undef make_wait_tmout
#undef wait_dequeue_wobj
#undef wait_complete
#undef wait_tmout
#undef wait_tmout_ok
#undef wobj_make_wait
#undef wobj_make_wait_tmout
#undef init_wait_queue

/*
 *  memory.c
 */
#undef initialize_memory
#undef search_meminib
#undef probe_mem_write
#undef probe_mem_read
#undef probe_mem_write_domptn
#undef probe_mem_rw_domptn
#undef initialize_sections

/*
 *  time_event.c
 */
#undef boundary_evttim
#undef current_evttim
#undef current_hrtcnt
#undef monotonic_evttim
#undef systim_offset
#undef in_signal_time
#undef initialize_tmevt
#undef tmevt_up
#undef tmevt_down
#undef update_current_evttim
#undef set_hrt_event
#undef tmevtb_register
#undef tmevtb_enqueue
#undef tmevtb_enqueue_reltim
#undef tmevtb_dequeue
#undef check_adjtim
#undef tmevt_lefttim
#undef tmevt_proc_top
#undef signal_time

/*
 *  semaphore.c
 */
#undef initialize_semaphore

/*
 *  eventflag.c
 */
#undef initialize_eventflag
#undef check_flg_cond

/*
 *  dataqueue.c
 */
#undef initialize_dataqueue
#undef enqueue_data
#undef force_enqueue_data
#undef dequeue_data
#undef send_data
#undef force_send_data
#undef receive_data

/*
 *  pridataq.c
 */
#undef initialize_pridataq
#undef enqueue_pridata
#undef dequeue_pridata
#undef send_pridata
#undef receive_pridata

/*
 *  mutex.c
 */
#undef initialize_mutex
#undef mutex_check_ceilpri
#undef mutex_scan_ceilmtx
#undef mutex_drop_priority
#undef mutex_acquire
#undef mutex_release
#undef mutex_release_all

/*
 *  messagebuf.c
 */
#undef initialize_messagebuf
#undef enqueue_message
#undef dequeue_message
#undef send_message
#undef messagebuf_signal
#undef receive_message
#undef messagebuf_dequeue_wobj

/*
 *  mempfix.c
 */
#undef initialize_mempfix
#undef get_mpf_block

/*
 *  time_manage.c
 */
#undef check_nfyinfo
#undef notify_handler

/*
 *  cyclic.c
 */
#undef initialize_cyclic
#undef call_cyclic

/*
 *  alarm.c
 */
#undef initialize_alarm
#undef call_alarm

/*
 *  interrupt.c
 */
#undef initialize_isr
#undef call_isr
#undef initialize_interrupt

/*
 *  exception.c
 */
#undef initialize_exception

/*
 *  svc_table.c
 */
#undef svc_table

/*
 *  kernel_cfg.c
 */
#undef tmax_tskid
#undef tmax_stskid
#undef tinib_table
#undef atinib_table
#undef torder_table
#undef tcb_table
#undef tmax_semid
#undef tmax_ssemid
#undef seminib_table
#undef aseminib_table
#undef semcb_table
#undef tmax_flgid
#undef tmax_sflgid
#undef flginib_table
#undef aflginib_table
#undef flgcb_table
#undef tmax_dtqid
#undef tmax_sdtqid
#undef dtqinib_table
#undef adtqinib_table
#undef dtqcb_table
#undef tmax_pdqid
#undef tmax_spdqid
#undef pdqinib_table
#undef apdqinib_table
#undef pdqcb_table
#undef tmax_mtxid
#undef tmax_smtxid
#undef mtxinib_table
#undef amtxinib_table
#undef mtxcb_table
#undef tmax_mbfid
#undef tmax_smbfid
#undef mbfinib_table
#undef ambfinib_table
#undef mbfcb_table
#undef tmax_mpfid
#undef tmax_smpfid
#undef mpfinib_table
#undef ampfinib_table
#undef mpfcb_table
#undef tmax_cycid
#undef tmax_scycid
#undef cycinib_table
#undef acycinib_table
#undef acyc_nfyinfo_table
#undef cyccb_table
#undef tmax_almid
#undef tmax_salmid
#undef alminib_table
#undef aalminib_table
#undef aalm_nfyinfo_table
#undef almcb_table
#undef tnum_isr_queue
#undef isr_queue_list
#undef isr_queue_table
#undef free_isrcb
#undef tmax_isrid
#undef tmax_sisrid
#undef isrinib_table
#undef aisrinib_table
#undef isrorder_table
#undef isrcb_table
#undef tnum_def_inhno
#undef inhinib_table
#undef tnum_cfg_intno
#undef intinib_table
#undef tnum_def_excno
#undef excinib_table
#undef tmax_fncd
#undef svcinib_table
#undef sysstat_acvct
#undef istksz
#undef istk
#undef istkpt
#undef tmevt_heap_kernel
#undef tmevt_heap_idle
#undef initialize_object
#undef tnum_inirtn
#undef inirtnb_table
#undef tnum_terrtn
#undef terrtnb_table
#undef system_cyctim
#undef tnum_schedcb
#undef schedcb_table
#undef twdinib_table
#undef tmax_somid
#undef p_inisom
#undef sominib_table
#undef tmax_domid
#undef domcb_kernel
#undef domcb_table
#undef domcb_none
#undef dominib_kernel
#undef dominib_table
#undef dominib_none

/*
 *  kernel_mem.c
 */
#undef tnum_meminib
#undef memtop_table
#undef meminib_table
#undef tnum_datasec
#undef datasecinib_table
#undef tnum_bsssec
#undef bsssecinib_table

/*
 *  tTraceLog.c
 */
#undef log_dsp_enter
#undef log_dsp_leave
#undef log_inh_enter
#undef log_inh_leave
#undef log_exc_enter
#undef log_exc_leave


#include "target_unrename.h"

#endif /* TOPPERS_KERNEL_RENAME_H */
