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
 *  $Id: alarm.c 502 2018-10-27 08:05:10Z ertl-hiro $
 */

/*
 *		アラーム通知機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "domain.h"
#include "alarm.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_ALM_ENTER
#define LOG_ALM_ENTER(p_almcb)
#endif /* LOG_ALM_ENTER */

#ifndef LOG_ALM_LEAVE
#define LOG_ALM_LEAVE(p_almcb)
#endif /* LOG_ALM_LEAVE */

#ifndef LOG_ACRE_ALM_ENTER
#define LOG_ACRE_ALM_ENTER(pk_calm)
#endif /* LOG_ACRE_ALM_ENTER */

#ifndef LOG_ACRE_ALM_LEAVE
#define LOG_ACRE_ALM_LEAVE(ercd)
#endif /* LOG_ACRE_ALM_LEAVE */

#ifndef LOG_SAC_ALM_ENTER
#define LOG_SAC_ALM_ENTER(almid, p_acvct)
#endif /* LOG_SAC_ALM_ENTER */

#ifndef LOG_SAC_ALM_LEAVE
#define LOG_SAC_ALM_LEAVE(ercd)
#endif /* LOG_SAC_ALM_LEAVE */

#ifndef LOG_DEL_ALM_ENTER
#define LOG_DEL_ALM_ENTER(almid)
#endif /* LOG_DEL_ALM_ENTER */

#ifndef LOG_DEL_ALM_LEAVE
#define LOG_DEL_ALM_LEAVE(ercd)
#endif /* LOG_DEL_ALM_LEAVE */

#ifndef LOG_STA_ALM_ENTER
#define LOG_STA_ALM_ENTER(almid, almtim)
#endif /* LOG_STA_ALM_ENTER */

#ifndef LOG_STA_ALM_LEAVE
#define LOG_STA_ALM_LEAVE(ercd)
#endif /* LOG_STA_ALM_LEAVE */

#ifndef LOG_STP_ALM_ENTER
#define LOG_STP_ALM_ENTER(almid)
#endif /* LOG_STP_ALM_ENTER */

#ifndef LOG_STP_ALM_LEAVE
#define LOG_STP_ALM_LEAVE(ercd)
#endif /* LOG_STP_ALM_LEAVE */

#ifndef LOG_REF_ALM_ENTER
#define LOG_REF_ALM_ENTER(almid, pk_ralm)
#endif /* LOG_REF_ALM_ENTER */

#ifndef LOG_REF_ALM_LEAVE
#define LOG_REF_ALM_LEAVE(ercd, pk_ralm)
#endif /* LOG_REF_ALM_LEAVE */

/*
 *  アラーム通知の数
 */
#define tnum_alm	((uint_t)(tmax_almid - TMIN_ALMID + 1))
#define tnum_salm	((uint_t)(tmax_salmid - TMIN_ALMID + 1))

/*
 *  アラーム通知IDからアラーム通知管理ブロックを取り出すためのマクロ
 */
#define INDEX_ALM(almid)	((uint_t)((almid) - TMIN_ALMID))
#define get_almcb(almid)	(&(almcb_table[INDEX_ALM(almid)]))

/*
 *  アラーム通知機能の初期化
 *
 *  未使用のアラーム通知管理ブロックのキューを作るために，アラーム通知
 *  管理ブロックの先頭にはキューにつなぐための領域がないため，タイムイ
 *  ベントブロック（tmevtb）の領域を用いる．
 */
#ifdef TOPPERS_almini

static void
initialize_almcb(ALMCB *p_almcb, ALMINIB *p_alminib, const DOMINIB *p_dominib)
{
	p_alminib->p_tmevt_heap = p_dominib->p_tmevt_heap;
	p_alminib->almatr = TA_NOEXS;
	p_almcb->p_alminib = (const ALMINIB *) p_alminib;
	p_almcb->tmevtb.callback = (CBACK) call_alarm;
	p_almcb->tmevtb.arg = (void *) p_almcb;
	queue_insert_prev(&(p_dominib->p_domcb->free_almcb),
								((QUEUE *) &(p_almcb->tmevtb)));
}

void
initialize_alarm(void)
{
	uint_t			i, j, k;
	ID				domid;
	ALMCB			*p_almcb;
	const DOMINIB	*p_dominib;

	for (i = 0; i < tnum_salm; i++) {
		p_almcb = &(almcb_table[i]);
		p_almcb->p_alminib = &(alminib_table[i]);
		p_almcb->almsta = false;
		p_almcb->tmevtb.callback = (CBACK) call_alarm;
		p_almcb->tmevtb.arg = (void *) p_almcb;
	}

	queue_initialize(&(dominib_kernel.p_domcb->free_almcb));
	for (j = 0; j < dominib_kernel.tnum_aalmid; i++, j++) {
		initialize_almcb(&(almcb_table[i]), &(aalminib_table[j]),
													&dominib_kernel);
	}
	for (domid = TMIN_DOMID; domid <= tmax_domid; domid++) {
		p_dominib = get_dominib(domid);
		queue_initialize(&(p_dominib->p_domcb->free_almcb));
		for (k = 0; k < p_dominib->tnum_aalmid; i++, j++, k++) {
			initialize_almcb(&(almcb_table[i]), &(aalminib_table[j]),
															p_dominib);
		}
	}
}

#endif /* TOPPERS_almini */

/*
 *  アラーム通知の生成
 */
#ifdef TOPPERS_acre_alm

ER_UINT
acre_alm(const T_CALM *pk_calm)
{
	ALMCB			*p_almcb;
	ALMINIB			*p_alminib;
	ATR				almatr;
	ID				domid;
	const DOMINIB	*p_dominib;
	T_NFYINFO		*p_nfyinfo;
	ACPTN			acptn;
	ER				ercd;

	LOG_ACRE_ALM_ENTER(pk_calm);
	CHECK_TSKCTX_UNL();
	CHECK_MACV_READ(pk_calm, T_CALM);

	almatr = pk_calm->almatr;

	CHECK_VALIDATR(almatr, TA_NULL|TA_DOMMASK);
	domid = get_atrdomid(almatr);
	if (domid == TDOM_SELF) {
		if (rundom == TACP_KERNEL) {
			domid = TDOM_KERNEL;
		}
		else {
			domid = p_runtsk->p_tinib->domid;
		}
		almatr = set_atrdomid(almatr, domid);
	}
	if (domid == TDOM_KERNEL) {
		p_dominib = &dominib_kernel;
	}
	else {
		CHECK_RSATR(VALID_DOMID(domid));
		p_dominib = get_dominib(domid);
	}
	ercd = check_nfyinfo(&(pk_calm->nfyinfo), p_dominib->domptn);
	if (ercd != E_OK) {
		goto error_exit;
	}
	CHECK_ACPTN(p_dominib->acvct.acptn1);
	
	lock_cpu();
	if (tnum_alm == 0 || queue_empty(&(p_dominib->p_domcb->free_almcb))) {
		ercd = E_NOID;
	}
	else {
		p_almcb = (ALMCB *)
			(((char *) queue_delete_next(&(p_dominib->p_domcb->free_almcb)))
													- offsetof(ALMCB, tmevtb));
		p_alminib = (ALMINIB *)(p_almcb->p_alminib);
		p_alminib->almatr = almatr;
		if (pk_calm->nfyinfo.nfymode == TNFY_HANDLER) {
			p_alminib->exinf = pk_calm->nfyinfo.nfy.handler.exinf;
			p_alminib->nfyhdr = (NFYHDR)(pk_calm->nfyinfo.nfy.handler.tmehdr);
		}
		else {
			p_nfyinfo = &aalm_nfyinfo_table[p_alminib - aalminib_table];
			*p_nfyinfo = pk_calm->nfyinfo;
			p_alminib->exinf = (intptr_t) p_nfyinfo;
			p_alminib->nfyhdr = notify_handler;
		}

		acptn = default_acptn(domid);
		p_alminib->acvct.acptn1 = acptn;
		p_alminib->acvct.acptn2 = acptn;
		p_alminib->acvct.acptn3 = p_dominib->acvct.acptn1;
		p_alminib->acvct.acptn4 = acptn;

		p_almcb->almsta = false;
		ercd = ALMID(p_almcb);
	}
	unlock_cpu();

  error_exit:
	LOG_ACRE_ALM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_acre_alm */

/*
 *  アラーム通知のアクセス許可ベクタの設定
 */
#ifdef TOPPERS_sac_alm

ER
sac_alm(ID almid, const ACVCT *p_acvct)
{
	ALMCB	*p_almcb;
	ALMINIB	*p_alminib;
	ER		ercd;

	LOG_SAC_ALM_ENTER(almid, p_acvct);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_ALMID(almid));
	CHECK_MACV_READ(p_acvct, ACVCT);
	p_almcb = get_almcb(almid);

	lock_cpu();
	if (p_almcb->p_alminib->almatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_almcb->p_alminib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (ALMID(p_almcb) <= tmax_salmid) {
		ercd = E_OBJ;
	}
	else {
		p_alminib = (ALMINIB *)(p_almcb->p_alminib);
		p_alminib->acvct = *p_acvct;
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_SAC_ALM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_sac_alm */

/*
 *  アラーム通知の削除
 */
#ifdef TOPPERS_del_alm

ER
del_alm(ID almid)
{
	ALMCB			*p_almcb;
	ALMINIB			*p_alminib;
	const DOMINIB	*p_dominib;
	ER				ercd;

	LOG_DEL_ALM_ENTER(almid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_ALMID(almid));
	p_almcb = get_almcb(almid);

	lock_cpu();
	if (p_almcb->p_alminib->almatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_almcb->p_alminib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (ALMID(p_almcb) <= tmax_salmid) {
		ercd = E_OBJ;
	}
	else {
		if (p_almcb->almsta) {
			p_almcb->almsta = false;
			tmevtb_dequeue(&(p_almcb->tmevtb),
									p_almcb->p_alminib->p_tmevt_heap);
		}

		p_alminib = (ALMINIB *)(p_almcb->p_alminib);
		p_dominib = get_atrdominib(p_alminib->almatr);
		p_alminib->almatr = TA_NOEXS;
		queue_insert_prev(&(p_dominib->p_domcb->free_almcb),
									((QUEUE *) &(p_almcb->tmevtb)));
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_DEL_ALM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_del_alm */

/*
 *  アラーム通知の動作開始
 */
#ifdef TOPPERS_sta_alm

ER
sta_alm(ID almid, RELTIM almtim)
{
	ALMCB	*p_almcb;
	ER		ercd;

	LOG_STA_ALM_ENTER(almid, almtim);
	CHECK_UNL();
	CHECK_ID(VALID_ALMID(almid));
	CHECK_PAR(VALID_RELTIM(almtim));
	p_almcb = get_almcb(almid);

	lock_cpu();
	if (p_almcb->p_alminib->almatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_almcb->p_alminib->acvct.acptn1)) {
		ercd = E_OACV;
	}
	else {
		if (p_almcb->almsta) {
			tmevtb_dequeue(&(p_almcb->tmevtb),
									p_almcb->p_alminib->p_tmevt_heap);
		}
		else {
			p_almcb->almsta = true;
		}
		tmevtb_enqueue_reltim(&(p_almcb->tmevtb), almtim,
									p_almcb->p_alminib->p_tmevt_heap);
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_STA_ALM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_sta_alm */

/*
 *  アラーム通知の動作停止
 */
#ifdef TOPPERS_stp_alm

ER
stp_alm(ID almid)
{
	ALMCB	*p_almcb;
	ER		ercd;

	LOG_STP_ALM_ENTER(almid);
	CHECK_UNL();
	CHECK_ID(VALID_ALMID(almid));
	p_almcb = get_almcb(almid);

	lock_cpu();
	if (p_almcb->p_alminib->almatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_almcb->p_alminib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else {
		if (p_almcb->almsta) {
			p_almcb->almsta = false;
			tmevtb_dequeue(&(p_almcb->tmevtb),
									p_almcb->p_alminib->p_tmevt_heap);
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_STP_ALM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_stp_alm */

/*
 *  アラーム通知の状態参照
 */
#ifdef TOPPERS_ref_alm

ER
ref_alm(ID almid, T_RALM *pk_ralm)
{
	ALMCB	*p_almcb;
	ER		ercd;
    
	LOG_REF_ALM_ENTER(almid, pk_ralm);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_ALMID(almid));
	CHECK_MACV_WRITE(pk_ralm, T_RALM);
	p_almcb = get_almcb(almid);

	lock_cpu();
	if (p_almcb->p_alminib->almatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_almcb->p_alminib->acvct.acptn4)) {
		ercd = E_OACV;
	}
	else {
		if (p_almcb->almsta) {
			pk_ralm->almstat = TALM_STA;
			pk_ralm->lefttim = tmevt_lefttim(&(p_almcb->tmevtb));
		}
		else {
			pk_ralm->almstat = TALM_STP;
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_REF_ALM_LEAVE(ercd, pk_ralm);
	return(ercd);
}

#endif /* TOPPERS_ref_alm */

/*
 *  アラーム通知起動ルーチン
 */
#ifdef TOPPERS_almcal

void
call_alarm(ALMCB *p_almcb)
{
	/*
	 *  アラーム通知を停止状態にする．
	 */
	p_almcb->almsta = false;

	/*
	 *  通知ハンドラを，CPUロック解除状態で呼び出す．
	 *
	 *  アラーム通知の生成／削除はタスクからしか行えないため，アラーム
	 *  通知初期化ブロックをCPUロック解除状態で参照しても問題ない．
	 */
	unlock_cpu();

	LOG_ALM_ENTER(p_almcb);
	(*(p_almcb->p_alminib->nfyhdr))(p_almcb->p_alminib->exinf);
	LOG_ALM_LEAVE(p_almcb);

	if (!sense_lock()) {
		lock_cpu();
	}
}

#endif /* TOPPERS_almcal */
