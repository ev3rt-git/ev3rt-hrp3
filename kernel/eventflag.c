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
 *  $Id: eventflag.c 520 2018-11-01 12:41:13Z ertl-hiro $
 */

/*
 *		イベントフラグ機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "domain.h"
#include "task.h"
#include "wait.h"
#include "eventflag.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_ACRE_FLG_ENTER
#define LOG_ACRE_FLG_ENTER(pk_cflg)
#endif /* LOG_ACRE_FLG_ENTER */

#ifndef LOG_ACRE_FLG_LEAVE
#define LOG_ACRE_FLG_LEAVE(ercd)
#endif /* LOG_ACRE_FLG_LEAVE */

#ifndef LOG_SAC_FLG_ENTER
#define LOG_SAC_FLG_ENTER(flgid, p_acvct)
#endif /* LOG_SAC_FLG_ENTER */

#ifndef LOG_SAC_FLG_LEAVE
#define LOG_SAC_FLG_LEAVE(ercd)
#endif /* LOG_SAC_FLG_LEAVE */

#ifndef LOG_DEL_FLG_ENTER
#define LOG_DEL_FLG_ENTER(flgid)
#endif /* LOG_DEL_FLG_ENTER */

#ifndef LOG_DEL_FLG_LEAVE
#define LOG_DEL_FLG_LEAVE(ercd)
#endif /* LOG_DEL_FLG_LEAVE */

#ifndef LOG_SET_FLG_ENTER
#define LOG_SET_FLG_ENTER(flgid, setptn)
#endif /* LOG_SET_FLG_ENTER */

#ifndef LOG_SET_FLG_LEAVE
#define LOG_SET_FLG_LEAVE(ercd)
#endif /* LOG_SET_FLG_LEAVE */

#ifndef LOG_CLR_FLG_ENTER
#define LOG_CLR_FLG_ENTER(flgid, clrptn)
#endif /* LOG_CLR_FLG_ENTER */

#ifndef LOG_CLR_FLG_LEAVE
#define LOG_CLR_FLG_LEAVE(ercd)
#endif /* LOG_CLR_FLG_LEAVE */

#ifndef LOG_WAI_FLG_ENTER
#define LOG_WAI_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn)
#endif /* LOG_WAI_FLG_ENTER */

#ifndef LOG_WAI_FLG_LEAVE
#define LOG_WAI_FLG_LEAVE(ercd, p_flgptn)
#endif /* LOG_WAI_FLG_LEAVE */

#ifndef LOG_POL_FLG_ENTER
#define LOG_POL_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn)
#endif /* LOG_POL_FLG_ENTER */

#ifndef LOG_POL_FLG_LEAVE
#define LOG_POL_FLG_LEAVE(ercd, p_flgptn)
#endif /* LOG_POL_FLG_LEAVE */

#ifndef LOG_TWAI_FLG_ENTER
#define LOG_TWAI_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn, tmout)
#endif /* LOG_TWAI_FLG_ENTER */

#ifndef LOG_TWAI_FLG_LEAVE
#define LOG_TWAI_FLG_LEAVE(ercd, p_flgptn)
#endif /* LOG_TWAI_FLG_LEAVE */

#ifndef LOG_INI_FLG_ENTER
#define LOG_INI_FLG_ENTER(flgid)
#endif /* LOG_INI_FLG_ENTER */

#ifndef LOG_INI_FLG_LEAVE
#define LOG_INI_FLG_LEAVE(ercd)
#endif /* LOG_INI_FLG_LEAVE */

#ifndef LOG_REF_FLG_ENTER
#define LOG_REF_FLG_ENTER(flgid, pk_rflg)
#endif /* LOG_REF_FLG_ENTER */

#ifndef LOG_REF_FLG_LEAVE
#define LOG_REF_FLG_LEAVE(ercd, pk_rflg)
#endif /* LOG_REF_FLG_LEAVE */

/*
 *  イベントフラグ機能の初期化
 */
#ifdef TOPPERS_flgini

static void
initialize_flgcb(FLGCB *p_flgcb, FLGINIB *p_flginib, const DOMINIB *p_dominib)
{
	p_flginib->flgatr = TA_NOEXS;
	p_flgcb->p_flginib = (const FLGINIB *) p_flginib;
	queue_insert_prev(&(p_dominib->p_domcb->free_flgcb),
										&(p_flgcb->wait_queue));
}

void
initialize_eventflag(void)
{
	uint_t			i, j, k;
	ID				domid;
	FLGCB			*p_flgcb;
	const DOMINIB	*p_dominib;

	for (i = 0; i < tnum_sflg; i++) {
		p_flgcb = &(flgcb_table[i]);
		queue_initialize(&(p_flgcb->wait_queue));
		p_flgcb->p_flginib = &(flginib_table[i]);
		p_flgcb->flgptn = p_flgcb->p_flginib->iflgptn;
	}

	queue_initialize(&(dominib_kernel.p_domcb->free_flgcb));
	for (j = 0; j < dominib_kernel.tnum_aflgid; i++, j++) {
		initialize_flgcb(&(flgcb_table[i]), &(aflginib_table[j]),
													&dominib_kernel);
	}
	for (domid = TMIN_DOMID; domid <= tmax_domid; domid++) {
		p_dominib = get_dominib(domid);
		queue_initialize(&(p_dominib->p_domcb->free_flgcb));
		for (k = 0; k < p_dominib->tnum_aflgid; i++, j++, k++) {
			initialize_flgcb(&(flgcb_table[i]), &(aflginib_table[j]),
															p_dominib);
		}
	}
	for (k = 0; k < dominib_none.tnum_aflgid; i++, j++, k++) {
		initialize_flgcb(&(flgcb_table[i]), &(aflginib_table[j]),
													&dominib_none);
	}
}

#endif /* TOPPERS_flgini */

/*
 *  イベントフラグ待ち解除条件のチェック
 */
#ifdef TOPPERS_flgcnd

bool_t
check_flg_cond(FLGCB *p_flgcb, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	if ((wfmode & TWF_ORW) != 0U ? (p_flgcb->flgptn & waiptn) != 0U
									: (p_flgcb->flgptn & waiptn) == waiptn) {
		*p_flgptn = p_flgcb->flgptn;
		if ((p_flgcb->p_flginib->flgatr & TA_CLR) != 0U) {
			p_flgcb->flgptn = 0U;
		}
		return(true);
	}
	return(false);
}

#endif /* TOPPERS_flgcnd */

/*
 *  イベントフラグの生成
 *
 *  pk_cflg->iflgptnは，エラーチェックをせず，一度しか参照しないため，
 *  ローカル変数にコピーする必要がない（途中で書き換わっても支障がな
 *  い）．
 */
#ifdef TOPPERS_acre_flg

ER_UINT
acre_flg(const T_CFLG *pk_cflg)
{
	FLGCB			*p_flgcb;
	FLGINIB			*p_flginib;
	ATR				flgatr;
	ID				domid;
	const DOMINIB	*p_dominib;
	ACPTN			acptn;
	ER				ercd;

	LOG_ACRE_FLG_ENTER(pk_cflg);
	CHECK_TSKCTX_UNL();
	CHECK_MACV_READ(pk_cflg, T_CFLG);

	flgatr = pk_cflg->flgatr;

	CHECK_VALIDATR(pk_cflg->flgatr, TA_TPRI|TA_WMUL|TA_CLR|TA_DOMMASK);
	domid = get_atrdomid(flgatr);
	if (domid == TDOM_SELF) {
		if (rundom == TACP_KERNEL) {
			domid = TDOM_KERNEL;
		}
		else {
			domid = p_runtsk->p_tinib->domid;
		}
		flgatr = set_atrdomid(flgatr, domid);
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
	if (tnum_flg == 0 || queue_empty(&(p_dominib->p_domcb->free_flgcb))) {
		ercd = E_NOID;
	}
	else {
		p_flgcb = (FLGCB *)
					queue_delete_next(&(p_dominib->p_domcb->free_flgcb));
		p_flginib = (FLGINIB *)(p_flgcb->p_flginib);
		p_flginib->flgatr = flgatr;
		p_flginib->iflgptn = pk_cflg->iflgptn;

		acptn = default_acptn(domid);
		p_flginib->acvct.acptn1 = acptn;
		p_flginib->acvct.acptn2 = acptn;
		p_flginib->acvct.acptn3 = p_dominib->acvct.acptn1;
		p_flginib->acvct.acptn4 = acptn;

		queue_initialize(&(p_flgcb->wait_queue));
		p_flgcb->flgptn = p_flgcb->p_flginib->iflgptn;
		ercd = FLGID(p_flgcb);
	}
	unlock_cpu();

  error_exit:
	LOG_ACRE_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_acre_flg */

/*
 *  イベントフラグのアクセス許可ベクタの設定
 */
#ifdef TOPPERS_sac_flg

ER
sac_flg(ID flgid, const ACVCT *p_acvct)
{
	FLGCB	*p_flgcb;
	FLGINIB	*p_flginib;
	ER		ercd;

	LOG_SAC_FLG_ENTER(flgid, p_acvct);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_FLGID(flgid));
	CHECK_MACV_READ(p_acvct, ACVCT);
	p_flgcb = get_flgcb(flgid);

	lock_cpu();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (FLGID(p_flgcb) <= tmax_sflgid) {
		ercd = E_OBJ;
	}
	else {
		p_flginib = (FLGINIB *)(p_flgcb->p_flginib);
		p_flginib->acvct = *p_acvct;
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_SAC_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_sac_flg */

/*
 *  イベントフラグの削除
 */
#ifdef TOPPERS_del_flg

ER
del_flg(ID flgid)
{
	FLGCB			*p_flgcb;
	FLGINIB			*p_flginib;
	const DOMINIB	*p_dominib;
	ER				ercd;

	LOG_DEL_FLG_ENTER(flgid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_FLGID(flgid));
	p_flgcb = get_flgcb(flgid);

	lock_cpu();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (FLGID(p_flgcb) <= tmax_sflgid) {
		ercd = E_OBJ;
	}
	else {
		init_wait_queue(&(p_flgcb->wait_queue));
		p_flginib = (FLGINIB *)(p_flgcb->p_flginib);
		p_dominib = get_atrdominib(p_flginib->flgatr);
		p_flginib->flgatr = TA_NOEXS;
		queue_insert_prev(&(p_dominib->p_domcb->free_flgcb),
										&(p_flgcb->wait_queue));
		if (p_runtsk != p_schedtsk) {
			dispatch();
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_DEL_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_del_flg */

/*
 *  イベントフラグのセット
 */
#ifdef TOPPERS_set_flg

ER
set_flg(ID flgid, FLGPTN setptn)
{
	FLGCB		*p_flgcb;
	QUEUE		*p_queue;
	TCB			*p_tcb;
	WINFO_FLG	*p_winfo_flg;
	ER			ercd;

	LOG_SET_FLG_ENTER(flgid, setptn);
	CHECK_UNL();
	CHECK_ID(VALID_FLGID(flgid));
	p_flgcb = get_flgcb(flgid);

	lock_cpu();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn1)) {
		ercd = E_OACV;
	}
	else {
		p_flgcb->flgptn |= setptn;
		p_queue = p_flgcb->wait_queue.p_next;
		while (p_queue != &(p_flgcb->wait_queue)) {
			p_tcb = (TCB *) p_queue;
			p_queue = p_queue->p_next;
			p_winfo_flg = (WINFO_FLG *)(p_tcb->p_winfo);
			if (check_flg_cond(p_flgcb, p_winfo_flg->waiptn,
							p_winfo_flg->wfmode, &(p_winfo_flg->waiptn))) {
				queue_delete(&(p_tcb->task_queue));
				wait_complete(p_tcb);
				if ((p_flgcb->p_flginib->flgatr & TA_CLR) != 0U) {
					break;
				}
			}
		}
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
	unlock_cpu();

  error_exit:
	LOG_SET_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_set_flg */

/*
 *  イベントフラグのクリア
 */
#ifdef TOPPERS_clr_flg

ER
clr_flg(ID flgid, FLGPTN clrptn)
{
	FLGCB	*p_flgcb;
	ER		ercd;

	LOG_CLR_FLG_ENTER(flgid, clrptn);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_FLGID(flgid));
	p_flgcb = get_flgcb(flgid);

	lock_cpu();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn1)) {
		ercd = E_OACV;
	}
	else {
		p_flgcb->flgptn &= clrptn; 
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_CLR_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_clr_flg */

/*
 *  イベントフラグ待ち
 */
#ifdef TOPPERS_wai_flg

ER
wai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	FLGCB		*p_flgcb;
	WINFO_FLG	winfo_flg;
	ER			ercd;

	LOG_WAI_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn);
	CHECK_DISPATCH();
	CHECK_ID(VALID_FLGID(flgid));
	CHECK_PAR(waiptn != 0U);
	CHECK_PAR(wfmode == TWF_ORW || wfmode == TWF_ANDW);
	CHECK_MACV_WRITE(p_flgptn, FLGPTN);
	p_flgcb = get_flgcb(flgid);

	lock_cpu_dsp();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else if (p_runtsk->raster) {
		ercd = E_RASTER;
	}
	else if ((p_flgcb->p_flginib->flgatr & TA_WMUL) == 0U
					&& !queue_empty(&(p_flgcb->wait_queue))) {
		ercd = E_ILUSE;
	}
	else if (check_flg_cond(p_flgcb, waiptn, wfmode, p_flgptn)) {
		ercd = E_OK;
	}
	else {
		winfo_flg.waiptn = waiptn;
		winfo_flg.wfmode = wfmode;
		wobj_make_wait((WOBJCB *) p_flgcb, TS_WAITING_FLG,
											(WINFO_WOBJ *) &winfo_flg);
		dispatch();
		ercd = winfo_flg.winfo.wercd;
		if (ercd == E_OK) {
			*p_flgptn = winfo_flg.waiptn;
		}
	}
	unlock_cpu_dsp();

  error_exit:
	LOG_WAI_FLG_LEAVE(ercd, p_flgptn);
	return(ercd);
}

#endif /* TOPPERS_wai_flg */

/*
 *  イベントフラグ待ち（ポーリング）
 */
#ifdef TOPPERS_pol_flg

ER
pol_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	FLGCB	*p_flgcb;
	ER		ercd;

	LOG_POL_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_FLGID(flgid));
	CHECK_PAR(waiptn != 0U);
	CHECK_PAR(wfmode == TWF_ORW || wfmode == TWF_ANDW);
	CHECK_MACV_WRITE(p_flgptn, FLGPTN);
	p_flgcb = get_flgcb(flgid);

	lock_cpu();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else if ((p_flgcb->p_flginib->flgatr & TA_WMUL) == 0U
					&& !queue_empty(&(p_flgcb->wait_queue))) {
		ercd = E_ILUSE;
	}
	else if (check_flg_cond(p_flgcb, waiptn, wfmode, p_flgptn)) {
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	unlock_cpu();

  error_exit:
	LOG_POL_FLG_LEAVE(ercd, p_flgptn);
	return(ercd);
}

#endif /* TOPPERS_pol_flg */

/*
 *  イベントフラグ待ち（タイムアウトあり）
 */
#ifdef TOPPERS_twai_flg

ER
twai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn, TMO tmout)
{
	FLGCB		*p_flgcb;
	WINFO_FLG	winfo_flg;
	TMEVTB		tmevtb;
	ER			ercd;

	LOG_TWAI_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn, tmout);
	CHECK_DISPATCH();
	CHECK_ID(VALID_FLGID(flgid));
	CHECK_PAR(waiptn != 0U);
	CHECK_PAR(wfmode == TWF_ORW || wfmode == TWF_ANDW);
	CHECK_MACV_WRITE(p_flgptn, FLGPTN);
	CHECK_PAR(VALID_TMOUT(tmout));
	p_flgcb = get_flgcb(flgid);

	lock_cpu_dsp();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else if (p_runtsk->raster) {
		ercd = E_RASTER;
	}
	else if ((p_flgcb->p_flginib->flgatr & TA_WMUL) == 0U
					&& !queue_empty(&(p_flgcb->wait_queue))) {
		ercd = E_ILUSE;
	}
	else if (check_flg_cond(p_flgcb, waiptn, wfmode, p_flgptn)) {
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		winfo_flg.waiptn = waiptn;
		winfo_flg.wfmode = wfmode;
		wobj_make_wait_tmout((WOBJCB *) p_flgcb, TS_WAITING_FLG,
								(WINFO_WOBJ *) &winfo_flg, &tmevtb, tmout);
		dispatch();
		ercd = winfo_flg.winfo.wercd;
		if (ercd == E_OK) {
			*p_flgptn = winfo_flg.waiptn;
		}
	}
	unlock_cpu_dsp();

  error_exit:
	LOG_TWAI_FLG_LEAVE(ercd, p_flgptn);
	return(ercd);
}

#endif /* TOPPERS_twai_flg */

/*
 *  イベントフラグの再初期化
 */
#ifdef TOPPERS_ini_flg

ER
ini_flg(ID flgid)
{
	FLGCB	*p_flgcb;
	ER		ercd;
    
	LOG_INI_FLG_ENTER(flgid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_FLGID(flgid));
	p_flgcb = get_flgcb(flgid);

	lock_cpu();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else {
		init_wait_queue(&(p_flgcb->wait_queue));
		p_flgcb->flgptn = p_flgcb->p_flginib->iflgptn;
		if (p_runtsk != p_schedtsk) {
			dispatch();
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_INI_FLG_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ini_flg */

/*
 *  イベントフラグの状態参照
 */
#ifdef TOPPERS_ref_flg

ER
ref_flg(ID flgid, T_RFLG *pk_rflg)
{
	FLGCB	*p_flgcb;
	ER		ercd;
    
	LOG_REF_FLG_ENTER(flgid, pk_rflg);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_FLGID(flgid));
	CHECK_MACV_WRITE(pk_rflg, T_RFLG);
	p_flgcb = get_flgcb(flgid);

	lock_cpu();
	if (p_flgcb->p_flginib->flgatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_flgcb->p_flginib->acvct.acptn4)) {
		ercd = E_OACV;
	}
	else {
		pk_rflg->wtskid = wait_tskid(&(p_flgcb->wait_queue));
		pk_rflg->flgptn = p_flgcb->flgptn;
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_REF_FLG_LEAVE(ercd, pk_rflg);
	return(ercd);
}

#endif /* TOPPERS_ref_flg */
