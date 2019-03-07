/*  This file is generated from svc.def by gensvc.rb. */

#ifndef TOPPERS_TOOL_SVC_H
#define TOPPERS_TOOL_SVC_H

#ifndef TOPPERS_MACRO_ONLY

Inline ER
act_tsk(ID tskid)
{
	CAL_SVC_1M(ER, TFN_ACT_TSK, ID, tskid);
}

Inline ER
get_tst(ID tskid, STAT *p_tskstat)
{
	CAL_SVC_2M(ER, TFN_GET_TST, ID, tskid, STAT *, p_tskstat);
}

Inline ER_UINT
can_act(ID tskid)
{
	CAL_SVC_1M(ER_UINT, TFN_CAN_ACT, ID, tskid);
}

Inline ER
ext_tsk(void)
{
	CAL_SVC_0M(ER, TFN_EXT_TSK);
}

Inline ER
ter_tsk(ID tskid)
{
	CAL_SVC_1M(ER, TFN_TER_TSK, ID, tskid);
}

Inline ER
chg_pri(ID tskid, PRI tskpri)
{
	CAL_SVC_2M(ER, TFN_CHG_PRI, ID, tskid, PRI, tskpri);
}

Inline ER
get_pri(ID tskid, PRI *p_tskpri)
{
	CAL_SVC_2M(ER, TFN_GET_PRI, ID, tskid, PRI *, p_tskpri);
}

Inline ER
get_inf(intptr_t *p_exinf)
{
	CAL_SVC_1M(ER, TFN_GET_INF, intptr_t *, p_exinf);
}

Inline ER
slp_tsk(void)
{
	CAL_SVC_0M(ER, TFN_SLP_TSK);
}

Inline ER
tslp_tsk(TMO tmout)
{
	CAL_SVC_1M(ER, TFN_TSLP_TSK, TMO, tmout);
}

Inline ER
wup_tsk(ID tskid)
{
	CAL_SVC_1M(ER, TFN_WUP_TSK, ID, tskid);
}

Inline ER_UINT
can_wup(ID tskid)
{
	CAL_SVC_1M(ER_UINT, TFN_CAN_WUP, ID, tskid);
}

Inline ER
rel_wai(ID tskid)
{
	CAL_SVC_1M(ER, TFN_REL_WAI, ID, tskid);
}

Inline ER
sus_tsk(ID tskid)
{
	CAL_SVC_1M(ER, TFN_SUS_TSK, ID, tskid);
}

Inline ER
rsm_tsk(ID tskid)
{
	CAL_SVC_1M(ER, TFN_RSM_TSK, ID, tskid);
}

Inline ER
dly_tsk(RELTIM dlytim)
{
	CAL_SVC_1M(ER, TFN_DLY_TSK, RELTIM, dlytim);
}

Inline ER
ras_ter(ID tskid)
{
	CAL_SVC_1M(ER, TFN_RAS_TER, ID, tskid);
}

Inline ER
dis_ter(void)
{
	CAL_SVC_0M(ER, TFN_DIS_TER);
}

Inline ER
ena_ter(void)
{
	CAL_SVC_0M(ER, TFN_ENA_TER);
}

Inline bool_t
sns_ter(void)
{
	CAL_SVC_0M(bool_t, TFN_SNS_TER);
}

Inline ER
sig_sem(ID semid)
{
	CAL_SVC_1M(ER, TFN_SIG_SEM, ID, semid);
}

Inline ER
wai_sem(ID semid)
{
	CAL_SVC_1M(ER, TFN_WAI_SEM, ID, semid);
}

Inline ER
pol_sem(ID semid)
{
	CAL_SVC_1M(ER, TFN_POL_SEM, ID, semid);
}

Inline ER
twai_sem(ID semid, TMO tmout)
{
	CAL_SVC_2M(ER, TFN_TWAI_SEM, ID, semid, TMO, tmout);
}

Inline ER
set_flg(ID flgid, FLGPTN setptn)
{
	CAL_SVC_2M(ER, TFN_SET_FLG, ID, flgid, FLGPTN, setptn);
}

Inline ER
clr_flg(ID flgid, FLGPTN clrptn)
{
	CAL_SVC_2M(ER, TFN_CLR_FLG, ID, flgid, FLGPTN, clrptn);
}

Inline ER
wai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	CAL_SVC_4M(ER, TFN_WAI_FLG, ID, flgid, FLGPTN, waiptn, MODE, wfmode, FLGPTN *, p_flgptn);
}

Inline ER
pol_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	CAL_SVC_4M(ER, TFN_POL_FLG, ID, flgid, FLGPTN, waiptn, MODE, wfmode, FLGPTN *, p_flgptn);
}

Inline ER
twai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn, TMO tmout)
{
	CAL_SVC_5M(ER, TFN_TWAI_FLG, ID, flgid, FLGPTN, waiptn, MODE, wfmode, FLGPTN *, p_flgptn, TMO, tmout);
}

Inline ER
snd_dtq(ID dtqid, intptr_t data)
{
	CAL_SVC_2M(ER, TFN_SND_DTQ, ID, dtqid, intptr_t, data);
}

Inline ER
psnd_dtq(ID dtqid, intptr_t data)
{
	CAL_SVC_2M(ER, TFN_PSND_DTQ, ID, dtqid, intptr_t, data);
}

Inline ER
tsnd_dtq(ID dtqid, intptr_t data, TMO tmout)
{
	CAL_SVC_3M(ER, TFN_TSND_DTQ, ID, dtqid, intptr_t, data, TMO, tmout);
}

Inline ER
fsnd_dtq(ID dtqid, intptr_t data)
{
	CAL_SVC_2M(ER, TFN_FSND_DTQ, ID, dtqid, intptr_t, data);
}

Inline ER
rcv_dtq(ID dtqid, intptr_t *p_data)
{
	CAL_SVC_2M(ER, TFN_RCV_DTQ, ID, dtqid, intptr_t *, p_data);
}

Inline ER
prcv_dtq(ID dtqid, intptr_t *p_data)
{
	CAL_SVC_2M(ER, TFN_PRCV_DTQ, ID, dtqid, intptr_t *, p_data);
}

Inline ER
trcv_dtq(ID dtqid, intptr_t *p_data, TMO tmout)
{
	CAL_SVC_3M(ER, TFN_TRCV_DTQ, ID, dtqid, intptr_t *, p_data, TMO, tmout);
}

Inline ER
snd_pdq(ID pdqid, intptr_t data, PRI datapri)
{
	CAL_SVC_3M(ER, TFN_SND_PDQ, ID, pdqid, intptr_t, data, PRI, datapri);
}

Inline ER
psnd_pdq(ID pdqid, intptr_t data, PRI datapri)
{
	CAL_SVC_3M(ER, TFN_PSND_PDQ, ID, pdqid, intptr_t, data, PRI, datapri);
}

Inline ER
tsnd_pdq(ID pdqid, intptr_t data, PRI datapri, TMO tmout)
{
	CAL_SVC_4M(ER, TFN_TSND_PDQ, ID, pdqid, intptr_t, data, PRI, datapri, TMO, tmout);
}

Inline ER
rcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri)
{
	CAL_SVC_3M(ER, TFN_RCV_PDQ, ID, pdqid, intptr_t *, p_data, PRI *, p_datapri);
}

Inline ER
prcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri)
{
	CAL_SVC_3M(ER, TFN_PRCV_PDQ, ID, pdqid, intptr_t *, p_data, PRI *, p_datapri);
}

Inline ER
trcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri, TMO tmout)
{
	CAL_SVC_4M(ER, TFN_TRCV_PDQ, ID, pdqid, intptr_t *, p_data, PRI *, p_datapri, TMO, tmout);
}

Inline ER
set_tim(SYSTIM systim)
{
	CAL_SVC_1M_SYSTIM(ER, TFN_SET_TIM, SYSTIM, systim);
}

Inline ER
get_tim(SYSTIM *p_systim)
{
	CAL_SVC_1M(ER, TFN_GET_TIM, SYSTIM *, p_systim);
}

Inline ER
adj_tim(int32_t adjtim)
{
	CAL_SVC_1M(ER, TFN_ADJ_TIM, int32_t, adjtim);
}

#ifdef TOPPERS_SUPPORT_DRIFT
Inline ER
set_dft(int32_t drift)
{
	CAL_SVC_1M(ER, TFN_SET_DFT, int32_t, drift);
}
#endif /* TOPPERS_SUPPORT_DRIFT */

Inline ER
loc_mtx(ID mtxid)
{
	CAL_SVC_1M(ER, TFN_LOC_MTX, ID, mtxid);
}

Inline ER
ploc_mtx(ID mtxid)
{
	CAL_SVC_1M(ER, TFN_PLOC_MTX, ID, mtxid);
}

Inline ER
tloc_mtx(ID mtxid, TMO tmout)
{
	CAL_SVC_2M(ER, TFN_TLOC_MTX, ID, mtxid, TMO, tmout);
}

Inline ER
unl_mtx(ID mtxid)
{
	CAL_SVC_1M(ER, TFN_UNL_MTX, ID, mtxid);
}

Inline ER
snd_mbf(ID mbfid, const void *msg, uint_t msgsz)
{
	CAL_SVC_3M(ER, TFN_SND_MBF, ID, mbfid, const void *, msg, uint_t, msgsz);
}

Inline ER
psnd_mbf(ID mbfid, const void *msg, uint_t msgsz)
{
	CAL_SVC_3M(ER, TFN_PSND_MBF, ID, mbfid, const void *, msg, uint_t, msgsz);
}

Inline ER
tsnd_mbf(ID mbfid, const void *msg, uint_t msgsz, TMO tmout)
{
	CAL_SVC_4M(ER, TFN_TSND_MBF, ID, mbfid, const void *, msg, uint_t, msgsz, TMO, tmout);
}

Inline ER_UINT
rcv_mbf(ID mbfid, void *msg)
{
	CAL_SVC_2M(ER_UINT, TFN_RCV_MBF, ID, mbfid, void *, msg);
}

Inline ER_UINT
prcv_mbf(ID mbfid, void *msg)
{
	CAL_SVC_2M(ER_UINT, TFN_PRCV_MBF, ID, mbfid, void *, msg);
}

Inline ER_UINT
trcv_mbf(ID mbfid, void *msg, TMO tmout)
{
	CAL_SVC_3M(ER_UINT, TFN_TRCV_MBF, ID, mbfid, void *, msg, TMO, tmout);
}

Inline ER
get_mpf(ID mpfid, void **p_blk)
{
	CAL_SVC_2M(ER, TFN_GET_MPF, ID, mpfid, void **, p_blk);
}

Inline ER
pget_mpf(ID mpfid, void **p_blk)
{
	CAL_SVC_2M(ER, TFN_PGET_MPF, ID, mpfid, void **, p_blk);
}

Inline ER
tget_mpf(ID mpfid, void **p_blk, TMO tmout)
{
	CAL_SVC_3M(ER, TFN_TGET_MPF, ID, mpfid, void **, p_blk, TMO, tmout);
}

Inline ER
rel_mpf(ID mpfid, void *blk)
{
	CAL_SVC_2M(ER, TFN_REL_MPF, ID, mpfid, void *, blk);
}

Inline HRTCNT
fch_hrt(void)
{
	CAL_SVC_0M(HRTCNT, TFN_FCH_HRT);
}

#ifdef TOPPERS_SUPPORT_OVRHDR
Inline ER
ref_ovr(ID tskid, T_ROVR *pk_rovr)
{
	CAL_SVC_2M(ER, TFN_REF_OVR, ID, tskid, T_ROVR *, pk_rovr);
}
#endif /* TOPPERS_SUPPORT_OVRHDR */

Inline ER
sta_cyc(ID cycid)
{
	CAL_SVC_1M(ER, TFN_STA_CYC, ID, cycid);
}

Inline ER
stp_cyc(ID cycid)
{
	CAL_SVC_1M(ER, TFN_STP_CYC, ID, cycid);
}

Inline ER
sta_alm(ID almid, RELTIM almtim)
{
	CAL_SVC_2M(ER, TFN_STA_ALM, ID, almid, RELTIM, almtim);
}

Inline ER
stp_alm(ID almid)
{
	CAL_SVC_1M(ER, TFN_STP_ALM, ID, almid);
}

#ifdef TOPPERS_SUPPORT_OVRHDR
Inline ER
sta_ovr(ID tskid, PRCTIM ovrtim)
{
	CAL_SVC_2M(ER, TFN_STA_OVR, ID, tskid, PRCTIM, ovrtim);
}
#endif /* TOPPERS_SUPPORT_OVRHDR */

#ifdef TOPPERS_SUPPORT_OVRHDR
Inline ER
stp_ovr(ID tskid)
{
	CAL_SVC_1M(ER, TFN_STP_OVR, ID, tskid);
}
#endif /* TOPPERS_SUPPORT_OVRHDR */

Inline ER
rot_rdq(PRI tskpri)
{
	CAL_SVC_1M(ER, TFN_ROT_RDQ, PRI, tskpri);
}

Inline ER
get_lod(PRI tskpri, uint_t *p_load)
{
	CAL_SVC_2M(ER, TFN_GET_LOD, PRI, tskpri, uint_t *, p_load);
}

Inline ER
get_did(ID *p_domid)
{
	CAL_SVC_1M(ER, TFN_GET_DID, ID *, p_domid);
}

Inline ER
get_tid(ID *p_tskid)
{
	CAL_SVC_1M(ER, TFN_GET_TID, ID *, p_tskid);
}

Inline ER
get_nth(PRI tskpri, uint_t nth, ID *p_tskid)
{
	CAL_SVC_3M(ER, TFN_GET_NTH, PRI, tskpri, uint_t, nth, ID *, p_tskid);
}

Inline ER
loc_cpu(void)
{
	CAL_SVC_0M(ER, TFN_LOC_CPU);
}

Inline ER
unl_cpu(void)
{
	CAL_SVC_0M(ER, TFN_UNL_CPU);
}

Inline ER
dis_dsp(void)
{
	CAL_SVC_0M(ER, TFN_DIS_DSP);
}

Inline ER
ena_dsp(void)
{
	CAL_SVC_0M(ER, TFN_ENA_DSP);
}

Inline bool_t
sns_ctx(void)
{
	CAL_SVC_0M(bool_t, TFN_SNS_CTX);
}

Inline bool_t
sns_loc(void)
{
	CAL_SVC_0M(bool_t, TFN_SNS_LOC);
}

Inline bool_t
sns_dsp(void)
{
	CAL_SVC_0M(bool_t, TFN_SNS_DSP);
}

Inline bool_t
sns_dpn(void)
{
	CAL_SVC_0M(bool_t, TFN_SNS_DPN);
}

Inline bool_t
sns_ker(void)
{
	CAL_SVC_0M(bool_t, TFN_SNS_KER);
}

Inline ER
ext_ker(void)
{
	CAL_SVC_0M(ER, TFN_EXT_KER);
}

Inline ER
prb_mem(const void *base, size_t size, ID tskid, MODE pmmode)
{
	CAL_SVC_4M(ER, TFN_PRB_MEM, const void *, base, size_t, size, ID, tskid, MODE, pmmode);
}

Inline ER
ref_mem(const void *addr, T_RMEM *pk_rmem)
{
	CAL_SVC_2M(ER, TFN_REF_MEM, const void *, addr, T_RMEM *, pk_rmem);
}

#ifdef TOPPERS_SUPPORT_DIS_INT
Inline ER
dis_int(INTNO intno)
{
	CAL_SVC_1M(ER, TFN_DIS_INT, INTNO, intno);
}
#endif /* TOPPERS_SUPPORT_DIS_INT */

#ifdef TOPPERS_SUPPORT_ENA_INT
Inline ER
ena_int(INTNO intno)
{
	CAL_SVC_1M(ER, TFN_ENA_INT, INTNO, intno);
}
#endif /* TOPPERS_SUPPORT_ENA_INT */

Inline ER
chg_ipm(PRI intpri)
{
	CAL_SVC_1M(ER, TFN_CHG_IPM, PRI, intpri);
}

Inline ER
get_ipm(PRI *p_intpri)
{
	CAL_SVC_1M(ER, TFN_GET_IPM, PRI *, p_intpri);
}

Inline bool_t
xsns_dpn(void *p_excinf)
{
	CAL_SVC_1M(bool_t, TFN_XSNS_DPN, void *, p_excinf);
}

#ifdef TOPPERS_SUPPORT_CLR_INT
Inline ER
clr_int(INTNO intno)
{
	CAL_SVC_1M(ER, TFN_CLR_INT, INTNO, intno);
}
#endif /* TOPPERS_SUPPORT_CLR_INT */

#ifdef TOPPERS_SUPPORT_RAS_INT
Inline ER
ras_int(INTNO intno)
{
	CAL_SVC_1M(ER, TFN_RAS_INT, INTNO, intno);
}
#endif /* TOPPERS_SUPPORT_RAS_INT */

#ifdef TOPPERS_SUPPORT_PRB_INT
Inline ER_BOOL
prb_int(INTNO intno)
{
	CAL_SVC_1M(ER_BOOL, TFN_PRB_INT, INTNO, intno);
}
#endif /* TOPPERS_SUPPORT_PRB_INT */

Inline ER
chg_som(ID somid)
{
	CAL_SVC_1M(ER, TFN_CHG_SOM, ID, somid);
}

Inline ER
get_som(ID *p_somid)
{
	CAL_SVC_1M(ER, TFN_GET_SOM, ID *, p_somid);
}

Inline ER
ini_sem(ID semid)
{
	CAL_SVC_1M(ER, TFN_INI_SEM, ID, semid);
}

Inline ER
ini_flg(ID flgid)
{
	CAL_SVC_1M(ER, TFN_INI_FLG, ID, flgid);
}

Inline ER
ini_dtq(ID dtqid)
{
	CAL_SVC_1M(ER, TFN_INI_DTQ, ID, dtqid);
}

Inline ER
ini_pdq(ID pdqid)
{
	CAL_SVC_1M(ER, TFN_INI_PDQ, ID, pdqid);
}

Inline ER
ini_mtx(ID mtxid)
{
	CAL_SVC_1M(ER, TFN_INI_MTX, ID, mtxid);
}

Inline ER
ini_mbf(ID mbfid)
{
	CAL_SVC_1M(ER, TFN_INI_MBF, ID, mbfid);
}

Inline ER
ini_mpf(ID mpfid)
{
	CAL_SVC_1M(ER, TFN_INI_MPF, ID, mpfid);
}

Inline ER
ref_tsk(ID tskid, T_RTSK *pk_rtsk)
{
	CAL_SVC_2M(ER, TFN_REF_TSK, ID, tskid, T_RTSK *, pk_rtsk);
}

Inline ER
ref_sem(ID semid, T_RSEM *pk_rsem)
{
	CAL_SVC_2M(ER, TFN_REF_SEM, ID, semid, T_RSEM *, pk_rsem);
}

Inline ER
ref_flg(ID flgid, T_RFLG *pk_rflg)
{
	CAL_SVC_2M(ER, TFN_REF_FLG, ID, flgid, T_RFLG *, pk_rflg);
}

Inline ER
ref_dtq(ID dtqid, T_RDTQ *pk_rdtq)
{
	CAL_SVC_2M(ER, TFN_REF_DTQ, ID, dtqid, T_RDTQ *, pk_rdtq);
}

Inline ER
ref_pdq(ID pdqid, T_RPDQ *pk_rpdq)
{
	CAL_SVC_2M(ER, TFN_REF_PDQ, ID, pdqid, T_RPDQ *, pk_rpdq);
}

Inline ER
ref_mtx(ID mtxid, T_RMTX *pk_rmtx)
{
	CAL_SVC_2M(ER, TFN_REF_MTX, ID, mtxid, T_RMTX *, pk_rmtx);
}

Inline ER
ref_mbf(ID mbfid, T_RMBF *pk_rmbf)
{
	CAL_SVC_2M(ER, TFN_REF_MBF, ID, mbfid, T_RMBF *, pk_rmbf);
}

Inline ER
ref_mpf(ID mpfid, T_RMPF *pk_rmpf)
{
	CAL_SVC_2M(ER, TFN_REF_MPF, ID, mpfid, T_RMPF *, pk_rmpf);
}

Inline ER
ref_cyc(ID cycid, T_RCYC *pk_rcyc)
{
	CAL_SVC_2M(ER, TFN_REF_CYC, ID, cycid, T_RCYC *, pk_rcyc);
}

Inline ER
ref_alm(ID almid, T_RALM *pk_ralm)
{
	CAL_SVC_2M(ER, TFN_REF_ALM, ID, almid, T_RALM *, pk_ralm);
}

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_tsk(const T_CTSK *pk_ctsk)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_TSK, const T_CTSK *, pk_ctsk);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_sem(const T_CSEM *pk_csem)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_SEM, const T_CSEM *, pk_csem);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_flg(const T_CFLG *pk_cflg)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_FLG, const T_CFLG *, pk_cflg);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_dtq(const T_CDTQ *pk_cdtq)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_DTQ, const T_CDTQ *, pk_cdtq);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_pdq(const T_CPDQ *pk_cpdq)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_PDQ, const T_CPDQ *, pk_cpdq);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_mtx(const T_CMTX *pk_cmtx)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_MTX, const T_CMTX *, pk_cmtx);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_mbf(const T_CMBF *pk_cmbf)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_MBF, const T_CMBF *, pk_cmbf);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_mpf(const T_CMPF *pk_cmpf)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_MPF, const T_CMPF *, pk_cmpf);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_cyc(const T_CCYC *pk_ccyc)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_CYC, const T_CCYC *, pk_ccyc);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_alm(const T_CALM *pk_calm)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_ALM, const T_CALM *, pk_calm);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER_ID
acre_isr(const T_CISR *pk_cisr)
{
	CAL_SVC_1M(ER_ID, TFN_ACRE_ISR, const T_CISR *, pk_cisr);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_tsk(ID tskid)
{
	CAL_SVC_1M(ER, TFN_DEL_TSK, ID, tskid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_sem(ID semid)
{
	CAL_SVC_1M(ER, TFN_DEL_SEM, ID, semid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_flg(ID flgid)
{
	CAL_SVC_1M(ER, TFN_DEL_FLG, ID, flgid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_dtq(ID dtqid)
{
	CAL_SVC_1M(ER, TFN_DEL_DTQ, ID, dtqid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_pdq(ID pdqid)
{
	CAL_SVC_1M(ER, TFN_DEL_PDQ, ID, pdqid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_mtx(ID mtxid)
{
	CAL_SVC_1M(ER, TFN_DEL_MTX, ID, mtxid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_mbf(ID mbfid)
{
	CAL_SVC_1M(ER, TFN_DEL_MBF, ID, mbfid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_mpf(ID mpfid)
{
	CAL_SVC_1M(ER, TFN_DEL_MPF, ID, mpfid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_cyc(ID cycid)
{
	CAL_SVC_1M(ER, TFN_DEL_CYC, ID, cycid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_alm(ID almid)
{
	CAL_SVC_1M(ER, TFN_DEL_ALM, ID, almid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
del_isr(ID isrid)
{
	CAL_SVC_1M(ER, TFN_DEL_ISR, ID, isrid);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_tsk(ID tskid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_TSK, ID, tskid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_sem(ID semid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_SEM, ID, semid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_flg(ID flgid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_FLG, ID, flgid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_dtq(ID dtqid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_DTQ, ID, dtqid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_pdq(ID pdqid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_PDQ, ID, pdqid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_mtx(ID mtxid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_MTX, ID, mtxid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_mbf(ID mbfid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_MBF, ID, mbfid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_mpf(ID mpfid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_MPF, ID, mpfid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_cyc(ID cycid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_CYC, ID, cycid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_alm(ID almid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_ALM, ID, almid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_DYNAMIC_CRE
Inline ER
sac_isr(ID isrid, const ACVCT *p_acvct)
{
	CAL_SVC_2M(ER, TFN_SAC_ISR, ID, isrid, const ACVCT *, p_acvct);
}
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifdef TOPPERS_SUPPORT_SUBPRIO
Inline ER
chg_spr(ID tskid, uint_t subpri)
{
	CAL_SVC_2M(ER, TFN_CHG_SPR, ID, tskid, uint_t, subpri);
}
#endif /* TOPPERS_SUPPORT_SUBPRIO */

Inline ER
mrot_rdq(ID schedno, PRI tskpri)
{
	CAL_SVC_2M(ER, TFN_MROT_RDQ, ID, schedno, PRI, tskpri);
}

Inline ER
mget_lod(ID schedno, PRI tskpri, uint_t *p_load)
{
	CAL_SVC_3M(ER, TFN_MGET_LOD, ID, schedno, PRI, tskpri, uint_t *, p_load);
}

Inline ER
mget_nth(ID schedno, PRI tskpri, uint_t nth, ID *p_tskid)
{
	CAL_SVC_4M(ER, TFN_MGET_NTH, ID, schedno, PRI, tskpri, uint_t, nth, ID *, p_tskid);
}

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TOOL_SVC_H */
