/*  This file is generated from svc.def by gensvc.rb. */

#include "kernel_impl.h"
#include "omit_svc.h"

static ER
no_support(void)
{
	return(E_NOSPT);
}

#ifndef TOPPERS_SUPPORT_DRIFT
#define set_dft no_support
#endif /* TOPPERS_SUPPORT_DRIFT */

#ifndef TOPPERS_SUPPORT_OVRHDR
#define ref_ovr no_support
#define sta_ovr no_support
#define stp_ovr no_support
#endif /* TOPPERS_SUPPORT_OVRHDR */

#ifndef TOPPERS_SUPPORT_DIS_INT
#define dis_int no_support
#endif /* TOPPERS_SUPPORT_DIS_INT */

#ifndef TOPPERS_SUPPORT_ENA_INT
#define ena_int no_support
#endif /* TOPPERS_SUPPORT_ENA_INT */

#ifndef TOPPERS_SUPPORT_CLR_INT
#define clr_int no_support
#endif /* TOPPERS_SUPPORT_CLR_INT */

#ifndef TOPPERS_SUPPORT_RAS_INT
#define ras_int no_support
#endif /* TOPPERS_SUPPORT_RAS_INT */

#ifndef TOPPERS_SUPPORT_PRB_INT
#define prb_int no_support
#endif /* TOPPERS_SUPPORT_PRB_INT */

#ifndef TOPPERS_SUPPORT_DYNAMIC_CRE
#define acre_tsk no_support
#define acre_sem no_support
#define acre_flg no_support
#define acre_dtq no_support
#define acre_pdq no_support
#define acre_mtx no_support
#define acre_mbf no_support
#define acre_mpf no_support
#define acre_cyc no_support
#define acre_alm no_support
#define acre_isr no_support
#define acre_spn no_support
#define del_tsk no_support
#define del_sem no_support
#define del_flg no_support
#define del_dtq no_support
#define del_pdq no_support
#define del_mtx no_support
#define del_mbf no_support
#define del_mpf no_support
#define del_cyc no_support
#define del_alm no_support
#define del_isr no_support
#define del_spn no_support
#define sac_tsk no_support
#define sac_sem no_support
#define sac_flg no_support
#define sac_dtq no_support
#define sac_pdq no_support
#define sac_mtx no_support
#define sac_mbf no_support
#define sac_mpf no_support
#define sac_cyc no_support
#define sac_alm no_support
#define sac_isr no_support
#define sac_spn no_support
#endif /* TOPPERS_SUPPORT_DYNAMIC_CRE */

#ifndef TOPPERS_SUPPORT_SUBPRIO
#define chg_spr no_support
#endif /* TOPPERS_SUPPORT_SUBPRIO */

typedef ER_UINT	(*SVC)();

const SVC svc_table[-TMIN_FNCD+1] = {
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(act_tsk),
	(SVC)(get_tst),
	(SVC)(can_act),
	(SVC)(ext_tsk),
	(SVC)(ter_tsk),
	(SVC)(chg_pri),
	(SVC)(get_pri),
	(SVC)(get_inf),
	(SVC)(slp_tsk),
	(SVC)(tslp_tsk),
	(SVC)(wup_tsk),
	(SVC)(no_support),
	(SVC)(can_wup),
	(SVC)(rel_wai),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(sus_tsk),
	(SVC)(rsm_tsk),
	(SVC)(dly_tsk),
	(SVC)(no_support),
	(SVC)(ras_ter),
	(SVC)(no_support),
	(SVC)(dis_ter),
	(SVC)(ena_ter),
	(SVC)(sns_ter),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(sig_sem),
	(SVC)(no_support),
	(SVC)(wai_sem),
	(SVC)(pol_sem),
	(SVC)(twai_sem),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(set_flg),
	(SVC)(no_support),
	(SVC)(clr_flg),
	(SVC)(wai_flg),
	(SVC)(pol_flg),
	(SVC)(twai_flg),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(snd_dtq),
	(SVC)(psnd_dtq),
	(SVC)(no_support),
	(SVC)(tsnd_dtq),
	(SVC)(fsnd_dtq),
	(SVC)(no_support),
	(SVC)(rcv_dtq),
	(SVC)(prcv_dtq),
	(SVC)(trcv_dtq),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(snd_pdq),
	(SVC)(psnd_pdq),
	(SVC)(no_support),
	(SVC)(tsnd_pdq),
	(SVC)(rcv_pdq),
	(SVC)(prcv_pdq),
	(SVC)(trcv_pdq),
	(SVC)(no_support),
	(SVC)(set_tim),
	(SVC)(get_tim),
	(SVC)(adj_tim),
	(SVC)(set_dft),
	(SVC)(loc_mtx),
	(SVC)(ploc_mtx),
	(SVC)(tloc_mtx),
	(SVC)(unl_mtx),
	(SVC)(snd_mbf),
	(SVC)(psnd_mbf),
	(SVC)(tsnd_mbf),
	(SVC)(rcv_mbf),
	(SVC)(prcv_mbf),
	(SVC)(trcv_mbf),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(get_mpf),
	(SVC)(pget_mpf),
	(SVC)(tget_mpf),
	(SVC)(rel_mpf),
	(SVC)(no_support),
	(SVC)(fch_hrt),
	(SVC)(no_support),
	(SVC)(ref_ovr),
	(SVC)(sta_cyc),
	(SVC)(stp_cyc),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(sta_alm),
	(SVC)(no_support),
	(SVC)(stp_alm),
	(SVC)(no_support),
	(SVC)(sta_ovr),
	(SVC)(no_support),
	(SVC)(stp_ovr),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(rot_rdq),
	(SVC)(get_lod),
	(SVC)(get_did),
	(SVC)(no_support),
	(SVC)(get_tid),
	(SVC)(get_nth),
	(SVC)(loc_cpu),
	(SVC)(no_support),
	(SVC)(unl_cpu),
	(SVC)(no_support),
	(SVC)(dis_dsp),
	(SVC)(ena_dsp),
	(SVC)(sns_ctx),
	(SVC)(sns_loc),
	(SVC)(sns_dsp),
	(SVC)(sns_dpn),
	(SVC)(sns_ker),
	(SVC)(ext_ker),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(prb_mem),
	(SVC)(ref_mem),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(dis_int),
	(SVC)(ena_int),
	(SVC)(no_support),
	(SVC)(chg_ipm),
	(SVC)(get_ipm),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(xsns_dpn),
	(SVC)(clr_int),
	(SVC)(ras_int),
	(SVC)(prb_int),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(chg_som),
	(SVC)(no_support),
	(SVC)(get_som),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(ini_sem),
	(SVC)(ini_flg),
	(SVC)(ini_dtq),
	(SVC)(ini_pdq),
	(SVC)(no_support),
	(SVC)(ini_mtx),
	(SVC)(ini_mbf),
	(SVC)(ini_mpf),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(ref_tsk),
	(SVC)(ref_sem),
	(SVC)(ref_flg),
	(SVC)(ref_dtq),
	(SVC)(ref_pdq),
	(SVC)(no_support),
	(SVC)(ref_mtx),
	(SVC)(ref_mbf),
	(SVC)(ref_mpf),
	(SVC)(ref_cyc),
	(SVC)(ref_alm),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(acre_tsk),
	(SVC)(acre_sem),
	(SVC)(acre_flg),
	(SVC)(acre_dtq),
	(SVC)(acre_pdq),
	(SVC)(no_support),
	(SVC)(acre_mtx),
	(SVC)(acre_mbf),
	(SVC)(acre_mpf),
	(SVC)(acre_cyc),
	(SVC)(acre_alm),
	(SVC)(acre_isr),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(del_tsk),
	(SVC)(del_sem),
	(SVC)(del_flg),
	(SVC)(del_dtq),
	(SVC)(del_pdq),
	(SVC)(no_support),
	(SVC)(del_mtx),
	(SVC)(del_mbf),
	(SVC)(del_mpf),
	(SVC)(del_cyc),
	(SVC)(del_alm),
	(SVC)(del_isr),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(sac_tsk),
	(SVC)(sac_sem),
	(SVC)(sac_flg),
	(SVC)(sac_dtq),
	(SVC)(sac_pdq),
	(SVC)(no_support),
	(SVC)(sac_mtx),
	(SVC)(sac_mbf),
	(SVC)(sac_mpf),
	(SVC)(sac_cyc),
	(SVC)(sac_alm),
	(SVC)(sac_isr),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(chg_spr),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(no_support),
	(SVC)(mrot_rdq),
	(SVC)(mget_lod),
	(SVC)(no_support),
	(SVC)(mget_nth),
};
