/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2018 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: semaphore.c 520 2018-11-01 12:41:13Z ertl-hiro $
 */

/*
 *		セマフォ機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "domain.h"
#include "task.h"
#include "wait.h"
#include "semaphore.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_ACRE_SEM_ENTER
#define LOG_ACRE_SEM_ENTER(pk_csem)
#endif /* LOG_ACRE_SEM_ENTER */

#ifndef LOG_ACRE_SEM_LEAVE
#define LOG_ACRE_SEM_LEAVE(ercd)
#endif /* LOG_ACRE_SEM_LEAVE */

#ifndef LOG_SAC_SEM_ENTER
#define LOG_SAC_SEM_ENTER(semid, p_acvct)
#endif /* LOG_SAC_SEM_ENTER */

#ifndef LOG_SAC_SEM_LEAVE
#define LOG_SAC_SEM_LEAVE(ercd)
#endif /* LOG_SAC_SEM_LEAVE */

#ifndef LOG_DEL_SEM_ENTER
#define LOG_DEL_SEM_ENTER(semid)
#endif /* LOG_DEL_SEM_ENTER */

#ifndef LOG_DEL_SEM_LEAVE
#define LOG_DEL_SEM_LEAVE(ercd)
#endif /* LOG_DEL_SEM_LEAVE */

#ifndef LOG_SIG_SEM_ENTER
#define LOG_SIG_SEM_ENTER(semid)
#endif /* LOG_SIG_SEM_ENTER */

#ifndef LOG_SIG_SEM_LEAVE
#define LOG_SIG_SEM_LEAVE(ercd)
#endif /* LOG_SIG_SEM_LEAVE */

#ifndef LOG_WAI_SEM_ENTER
#define LOG_WAI_SEM_ENTER(semid)
#endif /* LOG_WAI_SEM_ENTER */

#ifndef LOG_WAI_SEM_LEAVE
#define LOG_WAI_SEM_LEAVE(ercd)
#endif /* LOG_WAI_SEM_LEAVE */

#ifndef LOG_POL_SEM_ENTER
#define LOG_POL_SEM_ENTER(semid)
#endif /* LOG_POL_SEM_ENTER */

#ifndef LOG_POL_SEM_LEAVE
#define LOG_POL_SEM_LEAVE(ercd)
#endif /* LOG_POL_SEM_LEAVE */

#ifndef LOG_TWAI_SEM_ENTER
#define LOG_TWAI_SEM_ENTER(semid, tmout)
#endif /* LOG_TWAI_SEM_ENTER */

#ifndef LOG_TWAI_SEM_LEAVE
#define LOG_TWAI_SEM_LEAVE(ercd)
#endif /* LOG_TWAI_SEM_LEAVE */

#ifndef LOG_INI_SEM_ENTER
#define LOG_INI_SEM_ENTER(semid)
#endif /* LOG_INI_SEM_ENTER */

#ifndef LOG_INI_SEM_LEAVE
#define LOG_INI_SEM_LEAVE(ercd)
#endif /* LOG_INI_SEM_LEAVE */

#ifndef LOG_REF_SEM_ENTER
#define LOG_REF_SEM_ENTER(semid, pk_rsem)
#endif /* LOG_REF_SEM_ENTER */

#ifndef LOG_REF_SEM_LEAVE
#define LOG_REF_SEM_LEAVE(ercd, pk_rsem)
#endif /* LOG_REF_SEM_LEAVE */

/* 
 *  セマフォ機能の初期化
 */
#ifdef TOPPERS_semini

static void
initialize_semcb(SEMCB *p_semcb, SEMINIB *p_seminib, const DOMINIB *p_dominib)
{
	p_seminib->sematr = TA_NOEXS;
	p_semcb->p_seminib = (const SEMINIB *) p_seminib;
	queue_insert_prev(&(p_dominib->p_domcb->free_semcb),
										&(p_semcb->wait_queue));
}

void
initialize_semaphore(void)
{
	uint_t			i, j, k;
	ID				domid;
	SEMCB			*p_semcb;
	const DOMINIB	*p_dominib;

	for (i = 0; i < tnum_ssem; i++) {
		p_semcb = &(semcb_table[i]);
		queue_initialize(&(p_semcb->wait_queue));
		p_semcb->p_seminib = &(seminib_table[i]);
		p_semcb->semcnt = p_semcb->p_seminib->isemcnt;
	}

	queue_initialize(&(dominib_kernel.p_domcb->free_semcb));
	for (j = 0; j < dominib_kernel.tnum_asemid; i++, j++) {
		initialize_semcb(&(semcb_table[i]), &(aseminib_table[j]),
													&dominib_kernel);
	}
	for (domid = TMIN_DOMID; domid <= tmax_domid; domid++) {
		p_dominib = get_dominib(domid);
		queue_initialize(&(p_dominib->p_domcb->free_semcb));
		for (k = 0; k < p_dominib->tnum_asemid; i++, j++, k++) {
			initialize_semcb(&(semcb_table[i]), &(aseminib_table[j]),
															p_dominib);
		}
	}
	for (k = 0; k < dominib_none.tnum_asemid; i++, j++, k++) {
		initialize_semcb(&(semcb_table[i]), &(aseminib_table[j]),
													&dominib_none);
	}
}

#endif /* TOPPERS_semini */

/*
 *  セマフォの生成
 */
#ifdef TOPPERS_acre_sem

ER_UINT
acre_sem(const T_CSEM *pk_csem)
{
	SEMCB			*p_semcb;
	SEMINIB			*p_seminib;
	ATR				sematr;
	uint_t			isemcnt, maxsem;
	ID				domid;
	const DOMINIB	*p_dominib;
	ACPTN			acptn;
	ER				ercd;

	LOG_ACRE_SEM_ENTER(pk_csem);
	CHECK_TSKCTX_UNL();
	CHECK_MACV_READ(pk_csem, T_CSEM);

	sematr = pk_csem->sematr;
	isemcnt = pk_csem->isemcnt;
	maxsem = pk_csem->maxsem;

	CHECK_VALIDATR(sematr, TA_TPRI|TA_DOMMASK);
	CHECK_PAR(0 <= isemcnt && isemcnt <= maxsem);
	CHECK_PAR(1 <= maxsem && maxsem <= TMAX_MAXSEM);
	domid = get_atrdomid(sematr);
	if (domid == TDOM_SELF) {
		if (rundom == TACP_KERNEL) {
			domid = TDOM_KERNEL;
		}
		else {
			domid = p_runtsk->p_tinib->domid;
		}
		sematr = set_atrdomid(sematr, domid);
	}
	switch (domid) {
	case TDOM_KERNEL:
		p_dominib = &dominib_kernel;
		break;
	case TDOM_NONE:
		p_dominib = &dominib_none;
		break;
	default:
		CHECK_RSATR(VALID_DOMID(domid));
		p_dominib = get_dominib(domid);
		break;
	}
	CHECK_ACPTN(p_dominib->acvct.acptn1);

	lock_cpu();
	if (tnum_sem == 0 || queue_empty(&(p_dominib->p_domcb->free_semcb))) {
		ercd = E_NOID;
	}
	else {
		p_semcb = (SEMCB *)
					queue_delete_next(&(p_dominib->p_domcb->free_semcb));
		p_seminib = (SEMINIB *)(p_semcb->p_seminib);
		p_seminib->sematr = sematr;
		p_seminib->isemcnt = isemcnt;
		p_seminib->maxsem = maxsem;

		acptn = default_acptn(domid);
		p_seminib->acvct.acptn1 = acptn;
		p_seminib->acvct.acptn2 = acptn;
		p_seminib->acvct.acptn3 = p_dominib->acvct.acptn1;
		p_seminib->acvct.acptn4 = acptn;

		queue_initialize(&(p_semcb->wait_queue));
		p_semcb->semcnt = isemcnt;
		ercd = SEMID(p_semcb);
	}
	unlock_cpu();

  error_exit:
	LOG_ACRE_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_acre_sem */

/*
 *  セマフォのアクセス許可ベクタの設定
 */
#ifdef TOPPERS_sac_sem

ER
sac_sem(ID semid, const ACVCT *p_acvct)
{
	SEMCB	*p_semcb;
	SEMINIB	*p_seminib;
	ER		ercd;

	LOG_SAC_SEM_ENTER(semid, p_acvct);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_SEMID(semid));
	CHECK_MACV_READ(p_acvct, ACVCT);
	p_semcb = get_semcb(semid);

	lock_cpu();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (SEMID(p_semcb) <= tmax_ssemid) {
		ercd = E_OBJ;
	}
	else {
		p_seminib = (SEMINIB *)(p_semcb->p_seminib);
		p_seminib->acvct = *p_acvct;
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_SAC_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_sac_sem */

/*
 *  セマフォの削除
 */
#ifdef TOPPERS_del_sem

ER
del_sem(ID semid)
{
	SEMCB			*p_semcb;
	SEMINIB			*p_seminib;
	const DOMINIB	*p_dominib;
	ER				ercd;

	LOG_DEL_SEM_ENTER(semid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_SEMID(semid));
	p_semcb = get_semcb(semid);

	lock_cpu();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (SEMID(p_semcb) <= tmax_ssemid) {
		ercd = E_OBJ;
	}
	else {
		init_wait_queue(&(p_semcb->wait_queue));
		p_seminib = (SEMINIB *)(p_semcb->p_seminib);
		p_dominib = get_atrdominib(p_seminib->sematr);
		p_seminib->sematr = TA_NOEXS;
		queue_insert_prev(&(p_dominib->p_domcb->free_semcb),
										&(p_semcb->wait_queue));
		if (p_runtsk != p_schedtsk) {
			dispatch();
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_DEL_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_del_sem */

/*
 *  セマフォ資源の返却
 */
#ifdef TOPPERS_sig_sem

ER
sig_sem(ID semid)
{
	SEMCB	*p_semcb;
	TCB		*p_tcb;
	ER		ercd;
    
	LOG_SIG_SEM_ENTER(semid);
	CHECK_UNL();
	CHECK_ID(VALID_SEMID(semid));
	p_semcb = get_semcb(semid);

	lock_cpu();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn1)) {
		ercd = E_OACV;
	}
	else if (!queue_empty(&(p_semcb->wait_queue))) {
		p_tcb = (TCB *) queue_delete_next(&(p_semcb->wait_queue));
		wait_complete(p_tcb);
		if (p_runtsk != p_schedtsk) {
			if (!sense_context()) {
				dispatch();
			}
			else {
				request_dispatch_retint();
			}
		}
		ercd = E_OK;
	}
	else if (p_semcb->semcnt < p_semcb->p_seminib->maxsem) {
		p_semcb->semcnt += 1;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	unlock_cpu();

  error_exit:
	LOG_SIG_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_sig_sem */

/*
 *  セマフォ資源の獲得
 */
#ifdef TOPPERS_wai_sem

ER
wai_sem(ID semid)
{
	SEMCB		*p_semcb;
	WINFO_SEM	winfo_sem;
	ER			ercd;

	LOG_WAI_SEM_ENTER(semid);
	CHECK_DISPATCH();
	CHECK_ID(VALID_SEMID(semid));
	p_semcb = get_semcb(semid);

	lock_cpu_dsp();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else if (p_runtsk->raster) {
		ercd = E_RASTER;
	}
	else if (p_semcb->semcnt >= 1) {
		p_semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else {
		wobj_make_wait((WOBJCB *) p_semcb, TS_WAITING_SEM,
				 							(WINFO_WOBJ *) &winfo_sem);
		dispatch();
		ercd = winfo_sem.winfo.wercd;
	}
	unlock_cpu_dsp();

  error_exit:
	LOG_WAI_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_wai_sem */

/*
 *  セマフォ資源の獲得（ポーリング）
 */
#ifdef TOPPERS_pol_sem

ER
pol_sem(ID semid)
{
	SEMCB	*p_semcb;
	ER		ercd;

	LOG_POL_SEM_ENTER(semid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_SEMID(semid));
	p_semcb = get_semcb(semid);

	lock_cpu();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else if (p_semcb->semcnt >= 1) {
		p_semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	unlock_cpu();

  error_exit:
	LOG_POL_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_pol_sem */

/*
 *  セマフォ資源の獲得（タイムアウトあり）
 */
#ifdef TOPPERS_twai_sem

ER
twai_sem(ID semid, TMO tmout)
{
	SEMCB		*p_semcb;
	WINFO_SEM	winfo_sem;
	TMEVTB		tmevtb;
	ER			ercd;

	LOG_TWAI_SEM_ENTER(semid, tmout);
	CHECK_DISPATCH();
	CHECK_ID(VALID_SEMID(semid));
	CHECK_PAR(VALID_TMOUT(tmout));
	p_semcb = get_semcb(semid);

	lock_cpu_dsp();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else if (p_runtsk->raster) {
		ercd = E_RASTER;
	}
	else if (p_semcb->semcnt >= 1) {
		p_semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		wobj_make_wait_tmout((WOBJCB *) p_semcb, TS_WAITING_SEM,
								(WINFO_WOBJ *) &winfo_sem, &tmevtb, tmout);
		dispatch();
		ercd = winfo_sem.winfo.wercd;
	}
	unlock_cpu_dsp();

  error_exit:
	LOG_TWAI_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_twai_sem */

/*
 *  セマフォの再初期化
 */
#ifdef TOPPERS_ini_sem

ER
ini_sem(ID semid)
{
	SEMCB	*p_semcb;
	ER		ercd;
    
	LOG_INI_SEM_ENTER(semid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_SEMID(semid));
	p_semcb = get_semcb(semid);

	lock_cpu();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else {
		init_wait_queue(&(p_semcb->wait_queue));
		p_semcb->semcnt = p_semcb->p_seminib->isemcnt;
		if (p_runtsk != p_schedtsk) {
			dispatch();
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_INI_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ini_sem */

/*
 *  セマフォの状態参照
 */
#ifdef TOPPERS_ref_sem

ER
ref_sem(ID semid, T_RSEM *pk_rsem)
{
	SEMCB	*p_semcb;
	ER		ercd;
    
	LOG_REF_SEM_ENTER(semid, pk_rsem);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_SEMID(semid));
	CHECK_MACV_WRITE(pk_rsem, T_RSEM);
	p_semcb = get_semcb(semid);

	lock_cpu();
	if (p_semcb->p_seminib->sematr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_semcb->p_seminib->acvct.acptn4)) {
		ercd = E_OACV;
	}
	else {
		pk_rsem->wtskid = wait_tskid(&(p_semcb->wait_queue));
		pk_rsem->semcnt = p_semcb->semcnt;
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_REF_SEM_LEAVE(ercd, pk_rsem);
	return(ercd);
}

#endif /* TOPPERS_ref_sem */
