/*  This file is generated from svc.def by gensvc.rb. */

#ifndef TOPPERS_SVC_CALL_H
#define TOPPERS_SVC_CALL_H

#ifndef TOPPERS_MACRO_ONLY

extern ER _kernel_act_tsk(ID tskid) throw();
extern ER _kernel_get_tst(ID tskid, STAT *p_tskstat) throw();
extern ER_UINT _kernel_can_act(ID tskid) throw();
extern ER _kernel_ext_tsk(void) throw();
extern ER _kernel_ter_tsk(ID tskid) throw();
extern ER _kernel_chg_pri(ID tskid, PRI tskpri) throw();
extern ER _kernel_get_pri(ID tskid, PRI *p_tskpri) throw();
extern ER _kernel_get_inf(intptr_t *p_exinf) throw();
extern ER _kernel_slp_tsk(void) throw();
extern ER _kernel_tslp_tsk(TMO tmout) throw();
extern ER _kernel_wup_tsk(ID tskid) throw();
extern ER_UINT _kernel_can_wup(ID tskid) throw();
extern ER _kernel_rel_wai(ID tskid) throw();
extern ER _kernel_sus_tsk(ID tskid) throw();
extern ER _kernel_rsm_tsk(ID tskid) throw();
extern ER _kernel_dly_tsk(RELTIM dlytim) throw();
extern ER _kernel_ras_ter(ID tskid) throw();
extern ER _kernel_dis_ter(void) throw();
extern ER _kernel_ena_ter(void) throw();
extern bool_t _kernel_sns_ter(void) throw();
extern ER _kernel_sig_sem(ID semid) throw();
extern ER _kernel_wai_sem(ID semid) throw();
extern ER _kernel_pol_sem(ID semid) throw();
extern ER _kernel_twai_sem(ID semid, TMO tmout) throw();
extern ER _kernel_set_flg(ID flgid, FLGPTN setptn) throw();
extern ER _kernel_clr_flg(ID flgid, FLGPTN clrptn) throw();
extern ER _kernel_wai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn) throw();
extern ER _kernel_pol_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn) throw();
extern ER _kernel_twai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn, TMO tmout) throw();
extern ER _kernel_snd_dtq(ID dtqid, intptr_t data) throw();
extern ER _kernel_psnd_dtq(ID dtqid, intptr_t data) throw();
extern ER _kernel_tsnd_dtq(ID dtqid, intptr_t data, TMO tmout) throw();
extern ER _kernel_fsnd_dtq(ID dtqid, intptr_t data) throw();
extern ER _kernel_rcv_dtq(ID dtqid, intptr_t *p_data) throw();
extern ER _kernel_prcv_dtq(ID dtqid, intptr_t *p_data) throw();
extern ER _kernel_trcv_dtq(ID dtqid, intptr_t *p_data, TMO tmout) throw();
extern ER _kernel_snd_pdq(ID pdqid, intptr_t data, PRI datapri) throw();
extern ER _kernel_psnd_pdq(ID pdqid, intptr_t data, PRI datapri) throw();
extern ER _kernel_tsnd_pdq(ID pdqid, intptr_t data, PRI datapri, TMO tmout) throw();
extern ER _kernel_rcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri) throw();
extern ER _kernel_prcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri) throw();
extern ER _kernel_trcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri, TMO tmout) throw();
extern ER _kernel_set_tim(SYSTIM systim) throw();
extern ER _kernel_get_tim(SYSTIM *p_systim) throw();
extern ER _kernel_adj_tim(int32_t adjtim) throw();
#ifdef TOPPERS_SUPPORT_DRIFT
extern ER _kernel_set_dft(int32_t drift) throw();
#endif /* TOPPERS_SUPPORT_DRIFT */
extern ER _kernel_loc_mtx(ID mtxid) throw();
extern ER _kernel_ploc_mtx(ID mtxid) throw();
extern ER _kernel_tloc_mtx(ID mtxid, TMO tmout) throw();
extern ER _kernel_unl_mtx(ID mtxid) throw();
extern ER _kernel_snd_mbf(ID mbfid, const void *msg, uint_t msgsz) throw();
extern ER _kernel_psnd_mbf(ID mbfid, const void *msg, uint_t msgsz) throw();
extern ER _kernel_tsnd_mbf(ID mbfid, const void *msg, uint_t msgsz, TMO tmout) throw();
extern ER_UINT _kernel_rcv_mbf(ID mbfid, void *msg) throw();
extern ER_UINT _kernel_prcv_mbf(ID mbfid, void *msg) throw();
extern ER_UINT _kernel_trcv_mbf(ID mbfid, void *msg, TMO tmout) throw();
extern ER _kernel_get_mpf(ID mpfid, void **p_blk) throw();
extern ER _kernel_pget_mpf(ID mpfid, void **p_blk) throw();
extern ER _kernel_tget_mpf(ID mpfid, void **p_blk, TMO tmout) throw();
extern ER _kernel_rel_mpf(ID mpfid, void *blk) throw();
extern HRTCNT _kernel_fch_hrt(void) throw();
#ifdef TOPPERS_SUPPORT_OVRHDR
extern ER _kernel_ref_ovr(ID tskid, T_ROVR *pk_rovr) throw();
#endif /* TOPPERS_SUPPORT_OVRHDR */
extern ER _kernel_sta_cyc(ID cycid) throw();
extern ER _kernel_stp_cyc(ID cycid) throw();
extern ER _kernel_sta_alm(ID almid, RELTIM almtim) throw();
extern ER _kernel_stp_alm(ID almid) throw();
#ifdef TOPPERS_SUPPORT_OVRHDR
extern ER _kernel_sta_ovr(ID tskid, PRCTIM ovrtim) throw();
#endif /* TOPPERS_SUPPORT_OVRHDR */
#ifdef TOPPERS_SUPPORT_OVRHDR
extern ER _kernel_stp_ovr(ID tskid) throw();
#endif /* TOPPERS_SUPPORT_OVRHDR */
extern ER _kernel_rot_rdq(PRI tskpri) throw();
extern ER _kernel_get_lod(PRI tskpri, uint_t *p_load) throw();
extern ER _kernel_get_did(ID *p_domid) throw();
extern ER _kernel_get_tid(ID *p_tskid) throw();
extern ER _kernel_get_nth(PRI tskpri, uint_t nth, ID *p_tskid) throw();
extern ER _kernel_loc_cpu(void) throw();
extern ER _kernel_unl_cpu(void) throw();
extern ER _kernel_dis_dsp(void) throw();
extern ER _kernel_ena_dsp(void) throw();
extern bool_t _kernel_sns_ctx(void) throw();
extern bool_t _kernel_sns_loc(void) throw();
extern bool_t _kernel_sns_dsp(void) throw();
extern bool_t _kernel_sns_dpn(void) throw();
extern bool_t _kernel_sns_ker(void) throw();
extern ER _kernel_ext_ker(void) throw();
extern ER _kernel_prb_mem(const void *base, size_t size, ID tskid, MODE pmmode) throw();
extern ER _kernel_ref_mem(const void *addr, T_RMEM *pk_rmem) throw();
#ifdef TOPPERS_SUPPORT_DIS_INT
extern ER _kernel_dis_int(INTNO intno) throw();
#endif /* TOPPERS_SUPPORT_DIS_INT */
#ifdef TOPPERS_SUPPORT_ENA_INT
extern ER _kernel_ena_int(INTNO intno) throw();
#endif /* TOPPERS_SUPPORT_ENA_INT */
extern ER _kernel_chg_ipm(PRI intpri) throw();
extern ER _kernel_get_ipm(PRI *p_intpri) throw();
extern bool_t _kernel_xsns_dpn(void *p_excinf) throw();
#ifdef TOPPERS_SUPPORT_CLR_INT
extern ER _kernel_clr_int(INTNO intno) throw();
#endif /* TOPPERS_SUPPORT_CLR_INT */
#ifdef TOPPERS_SUPPORT_RAS_INT
extern ER _kernel_ras_int(INTNO intno) throw();
#endif /* TOPPERS_SUPPORT_RAS_INT */
#ifdef TOPPERS_SUPPORT_PRB_INT
extern ER_BOOL _kernel_prb_int(INTNO intno) throw();
#endif /* TOPPERS_SUPPORT_PRB_INT */
extern ER _kernel_chg_som(ID somid) throw();
extern ER _kernel_get_som(ID *p_somid) throw();
extern ER _kernel_ini_sem(ID semid) throw();
extern ER _kernel_ini_flg(ID flgid) throw();
extern ER _kernel_ini_dtq(ID dtqid) throw();
extern ER _kernel_ini_pdq(ID pdqid) throw();
extern ER _kernel_ini_mtx(ID mtxid) throw();
extern ER _kernel_ini_mbf(ID mbfid) throw();
extern ER _kernel_ini_mpf(ID mpfid) throw();
extern ER _kernel_ref_tsk(ID tskid, T_RTSK *pk_rtsk) throw();
extern ER _kernel_ref_sem(ID semid, T_RSEM *pk_rsem) throw();
extern ER _kernel_ref_flg(ID flgid, T_RFLG *pk_rflg) throw();
extern ER _kernel_ref_dtq(ID dtqid, T_RDTQ *pk_rdtq) throw();
extern ER _kernel_ref_pdq(ID pdqid, T_RPDQ *pk_rpdq) throw();
extern ER _kernel_ref_mtx(ID mtxid, T_RMTX *pk_rmtx) throw();
extern ER _kernel_ref_mbf(ID mbfid, T_RMBF *pk_rmbf) throw();
extern ER _kernel_ref_mpf(ID mpfid, T_RMPF *pk_rmpf) throw();
extern ER _kernel_ref_cyc(ID cycid, T_RCYC *pk_rcyc) throw();
extern ER _kernel_ref_alm(ID almid, T_RALM *pk_ralm) throw();
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_tsk(const T_CTSK *pk_ctsk) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_sem(const T_CSEM *pk_csem) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_flg(const T_CFLG *pk_cflg) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_dtq(const T_CDTQ *pk_cdtq) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_pdq(const T_CPDQ *pk_cpdq) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_mtx(const T_CMTX *pk_cmtx) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_mbf(const T_CMBF *pk_cmbf) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_mpf(const T_CMPF *pk_cmpf) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_cyc(const T_CCYC *pk_ccyc) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_alm(const T_CALM *pk_calm) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER_ID _kernel_acre_isr(const T_CISR *pk_cisr) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_tsk(ID tskid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_sem(ID semid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_flg(ID flgid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_dtq(ID dtqid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_pdq(ID pdqid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_mtx(ID mtxid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_mbf(ID mbfid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_mpf(ID mpfid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_cyc(ID cycid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_alm(ID almid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_del_isr(ID isrid) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_tsk(ID tskid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_sem(ID semid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_flg(ID flgid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_dtq(ID dtqid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_pdq(ID pdqid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_mtx(ID mtxid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_mbf(ID mbfid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_mpf(ID mpfid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_cyc(ID cycid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_alm(ID almid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
extern ER _kernel_sac_isr(ID isrid, const ACVCT *p_acvct) throw();
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_SUBPRIO
extern ER _kernel_chg_spr(ID tskid, uint_t subpri) throw();
#endif /* TOPPERS_SUPPORT_SUBPRIO */
extern ER _kernel_mrot_rdq(ID schedno, PRI tskpri) throw();
extern ER _kernel_mget_lod(ID schedno, PRI tskpri, uint_t *p_load) throw();
extern ER _kernel_mget_nth(ID schedno, PRI tskpri, uint_t nth, ID *p_tskid) throw();

#endif /* TOPPERS_MACRO_ONLY */

#ifdef TOPPERS_SVC_CALL

#define act_tsk _kernel_act_tsk
#define get_tst _kernel_get_tst
#define can_act _kernel_can_act
#define ext_tsk _kernel_ext_tsk
#define ter_tsk _kernel_ter_tsk
#define chg_pri _kernel_chg_pri
#define get_pri _kernel_get_pri
#define get_inf _kernel_get_inf
#define slp_tsk _kernel_slp_tsk
#define tslp_tsk _kernel_tslp_tsk
#define wup_tsk _kernel_wup_tsk
#define can_wup _kernel_can_wup
#define rel_wai _kernel_rel_wai
#define sus_tsk _kernel_sus_tsk
#define rsm_tsk _kernel_rsm_tsk
#define dly_tsk _kernel_dly_tsk
#define ras_ter _kernel_ras_ter
#define dis_ter _kernel_dis_ter
#define ena_ter _kernel_ena_ter
#define sns_ter _kernel_sns_ter
#define sig_sem _kernel_sig_sem
#define wai_sem _kernel_wai_sem
#define pol_sem _kernel_pol_sem
#define twai_sem _kernel_twai_sem
#define set_flg _kernel_set_flg
#define clr_flg _kernel_clr_flg
#define wai_flg _kernel_wai_flg
#define pol_flg _kernel_pol_flg
#define twai_flg _kernel_twai_flg
#define snd_dtq _kernel_snd_dtq
#define psnd_dtq _kernel_psnd_dtq
#define tsnd_dtq _kernel_tsnd_dtq
#define fsnd_dtq _kernel_fsnd_dtq
#define rcv_dtq _kernel_rcv_dtq
#define prcv_dtq _kernel_prcv_dtq
#define trcv_dtq _kernel_trcv_dtq
#define snd_pdq _kernel_snd_pdq
#define psnd_pdq _kernel_psnd_pdq
#define tsnd_pdq _kernel_tsnd_pdq
#define rcv_pdq _kernel_rcv_pdq
#define prcv_pdq _kernel_prcv_pdq
#define trcv_pdq _kernel_trcv_pdq
#define set_tim _kernel_set_tim
#define get_tim _kernel_get_tim
#define adj_tim _kernel_adj_tim
#ifdef TOPPERS_SUPPORT_DRIFT
#define set_dft _kernel_set_dft
#endif /* TOPPERS_SUPPORT_DRIFT */
#define loc_mtx _kernel_loc_mtx
#define ploc_mtx _kernel_ploc_mtx
#define tloc_mtx _kernel_tloc_mtx
#define unl_mtx _kernel_unl_mtx
#define snd_mbf _kernel_snd_mbf
#define psnd_mbf _kernel_psnd_mbf
#define tsnd_mbf _kernel_tsnd_mbf
#define rcv_mbf _kernel_rcv_mbf
#define prcv_mbf _kernel_prcv_mbf
#define trcv_mbf _kernel_trcv_mbf
#define get_mpf _kernel_get_mpf
#define pget_mpf _kernel_pget_mpf
#define tget_mpf _kernel_tget_mpf
#define rel_mpf _kernel_rel_mpf
#define fch_hrt _kernel_fch_hrt
#ifdef TOPPERS_SUPPORT_OVRHDR
#define ref_ovr _kernel_ref_ovr
#endif /* TOPPERS_SUPPORT_OVRHDR */
#define sta_cyc _kernel_sta_cyc
#define stp_cyc _kernel_stp_cyc
#define sta_alm _kernel_sta_alm
#define stp_alm _kernel_stp_alm
#ifdef TOPPERS_SUPPORT_OVRHDR
#define sta_ovr _kernel_sta_ovr
#endif /* TOPPERS_SUPPORT_OVRHDR */
#ifdef TOPPERS_SUPPORT_OVRHDR
#define stp_ovr _kernel_stp_ovr
#endif /* TOPPERS_SUPPORT_OVRHDR */
#define rot_rdq _kernel_rot_rdq
#define get_lod _kernel_get_lod
#define get_did _kernel_get_did
#define get_tid _kernel_get_tid
#define get_nth _kernel_get_nth
#define loc_cpu _kernel_loc_cpu
#define unl_cpu _kernel_unl_cpu
#define dis_dsp _kernel_dis_dsp
#define ena_dsp _kernel_ena_dsp
#define sns_ctx _kernel_sns_ctx
#define sns_loc _kernel_sns_loc
#define sns_dsp _kernel_sns_dsp
#define sns_dpn _kernel_sns_dpn
#define sns_ker _kernel_sns_ker
#define ext_ker _kernel_ext_ker
#define prb_mem _kernel_prb_mem
#define ref_mem _kernel_ref_mem
#ifdef TOPPERS_SUPPORT_DIS_INT
#define dis_int _kernel_dis_int
#endif /* TOPPERS_SUPPORT_DIS_INT */
#ifdef TOPPERS_SUPPORT_ENA_INT
#define ena_int _kernel_ena_int
#endif /* TOPPERS_SUPPORT_ENA_INT */
#define chg_ipm _kernel_chg_ipm
#define get_ipm _kernel_get_ipm
#define xsns_dpn _kernel_xsns_dpn
#ifdef TOPPERS_SUPPORT_CLR_INT
#define clr_int _kernel_clr_int
#endif /* TOPPERS_SUPPORT_CLR_INT */
#ifdef TOPPERS_SUPPORT_RAS_INT
#define ras_int _kernel_ras_int
#endif /* TOPPERS_SUPPORT_RAS_INT */
#ifdef TOPPERS_SUPPORT_PRB_INT
#define prb_int _kernel_prb_int
#endif /* TOPPERS_SUPPORT_PRB_INT */
#define chg_som _kernel_chg_som
#define get_som _kernel_get_som
#define ini_sem _kernel_ini_sem
#define ini_flg _kernel_ini_flg
#define ini_dtq _kernel_ini_dtq
#define ini_pdq _kernel_ini_pdq
#define ini_mtx _kernel_ini_mtx
#define ini_mbf _kernel_ini_mbf
#define ini_mpf _kernel_ini_mpf
#define ref_tsk _kernel_ref_tsk
#define ref_sem _kernel_ref_sem
#define ref_flg _kernel_ref_flg
#define ref_dtq _kernel_ref_dtq
#define ref_pdq _kernel_ref_pdq
#define ref_mtx _kernel_ref_mtx
#define ref_mbf _kernel_ref_mbf
#define ref_mpf _kernel_ref_mpf
#define ref_cyc _kernel_ref_cyc
#define ref_alm _kernel_ref_alm
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_tsk _kernel_acre_tsk
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_sem _kernel_acre_sem
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_flg _kernel_acre_flg
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_dtq _kernel_acre_dtq
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_pdq _kernel_acre_pdq
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_mtx _kernel_acre_mtx
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_mbf _kernel_acre_mbf
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_mpf _kernel_acre_mpf
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_cyc _kernel_acre_cyc
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_alm _kernel_acre_alm
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_isr _kernel_acre_isr
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_tsk _kernel_del_tsk
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_sem _kernel_del_sem
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_flg _kernel_del_flg
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_dtq _kernel_del_dtq
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_pdq _kernel_del_pdq
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_mtx _kernel_del_mtx
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_mbf _kernel_del_mbf
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_mpf _kernel_del_mpf
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_cyc _kernel_del_cyc
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_alm _kernel_del_alm
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define del_isr _kernel_del_isr
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_tsk _kernel_sac_tsk
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_sem _kernel_sac_sem
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_flg _kernel_sac_flg
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_dtq _kernel_sac_dtq
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_pdq _kernel_sac_pdq
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_mtx _kernel_sac_mtx
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_mbf _kernel_sac_mbf
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_mpf _kernel_sac_mpf
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_cyc _kernel_sac_cyc
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_alm _kernel_sac_alm
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
#define sac_isr _kernel_sac_isr
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */
#ifdef TOPPERS_SUPPORT_SUBPRIO
#define chg_spr _kernel_chg_spr
#endif /* TOPPERS_SUPPORT_SUBPRIO */
#define mrot_rdq _kernel_mrot_rdq
#define mget_lod _kernel_mget_lod
#define mget_nth _kernel_mget_nth

#endif /* TOPPERS_SVC_CALL */
#endif /* TOPPERS_SVC_CALL_H */
