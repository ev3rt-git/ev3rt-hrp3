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
 *  $Id: mempfix.c 502 2018-10-27 08:05:10Z ertl-hiro $
 */

/*
 *		固定長メモリプール機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "domain.h"
#include "task.h"
#include "wait.h"
#include "mempfix.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_ACRE_MPF_ENTER
#define LOG_ACRE_MPF_ENTER(pk_cmpf)
#endif /* LOG_ACRE_MPF_ENTER */

#ifndef LOG_ACRE_MPF_LEAVE
#define LOG_ACRE_MPF_LEAVE(ercd)
#endif /* LOG_ACRE_MPF_LEAVE */

#ifndef LOG_SAC_MPF_ENTER
#define LOG_SAC_MPF_ENTER(mpfid, p_acvct)
#endif /* LOG_SAC_MPF_ENTER */

#ifndef LOG_SAC_MPF_LEAVE
#define LOG_SAC_MPF_LEAVE(ercd)
#endif /* LOG_SAC_MPF_LEAVE */

#ifndef LOG_DEL_MPF_ENTER
#define LOG_DEL_MPF_ENTER(mpfid)
#endif /* LOG_DEL_MPF_ENTER */

#ifndef LOG_DEL_MPF_LEAVE
#define LOG_DEL_MPF_LEAVE(ercd)
#endif /* LOG_DEL_MPF_LEAVE */

#ifndef LOG_GET_MPF_ENTER
#define LOG_GET_MPF_ENTER(mpfid, p_blk)
#endif /* LOG_GET_MPF_ENTER */

#ifndef LOG_GET_MPF_LEAVE
#define LOG_GET_MPF_LEAVE(ercd, p_blk)
#endif /* LOG_GET_MPF_LEAVE */

#ifndef LOG_PGET_MPF_ENTER
#define LOG_PGET_MPF_ENTER(mpfid, p_blk)
#endif /* LOG_PGET_MPF_ENTER */

#ifndef LOG_PGET_MPF_LEAVE
#define LOG_PGET_MPF_LEAVE(ercd, p_blk)
#endif /* LOG_PGET_MPF_LEAVE */

#ifndef LOG_TGET_MPF_ENTER
#define LOG_TGET_MPF_ENTER(mpfid, p_blk, tmout)
#endif /* LOG_TGET_MPF_ENTER */

#ifndef LOG_TGET_MPF_LEAVE
#define LOG_TGET_MPF_LEAVE(ercd, p_blk)
#endif /* LOG_TGET_MPF_LEAVE */

#ifndef LOG_REL_MPF_ENTER
#define LOG_REL_MPF_ENTER(mpfid, blk)
#endif /* LOG_REL_MPF_ENTER */

#ifndef LOG_REL_MPF_LEAVE
#define LOG_REL_MPF_LEAVE(ercd)
#endif /* LOG_REL_MPF_LEAVE */

#ifndef LOG_INI_MPF_ENTER
#define LOG_INI_MPF_ENTER(mpfid)
#endif /* LOG_INI_MPF_ENTER */

#ifndef LOG_INI_MPF_LEAVE
#define LOG_INI_MPF_LEAVE(ercd)
#endif /* LOG_INI_MPF_LEAVE */

#ifndef LOG_REF_MPF_ENTER
#define LOG_REF_MPF_ENTER(mpfid, pk_rmpf)
#endif /* LOG_REF_MPF_ENTER */

#ifndef LOG_REF_MPF_LEAVE
#define LOG_REF_MPF_LEAVE(ercd, pk_rmpf)
#endif /* LOG_REF_MPF_LEAVE */

/*
 *  固定長メモリプールの数
 */
#define tnum_mpf	((uint_t)(tmax_mpfid - TMIN_MPFID + 1))
#define tnum_smpf	((uint_t)(tmax_smpfid - TMIN_MPFID + 1))

/*
 *  固定長メモリプールIDから固定長メモリプール管理ブロックを取り出すた
 *  めのマクロ
 */
#define INDEX_MPF(mpfid)	((uint_t)((mpfid) - TMIN_MPFID))
#define get_mpfcb(mpfid)	(&(mpfcb_table[INDEX_MPF(mpfid)]))

/*
 *  特殊なインデックス値の定義
 */
#define INDEX_NULL		(~0U)		/* 空きブロックリストの最後 */
#define INDEX_ALLOC		(~1U)		/* 割当て済みのブロック */

/*
 *  固定長メモリプール機能の初期化
 */
#ifdef TOPPERS_mpfini

static void
initialize_mpfcb(MPFCB *p_mpfcb, MPFINIB *p_mpfinib, const DOMINIB *p_dominib)
{
	p_mpfinib->mpfatr = TA_NOEXS;
	p_mpfcb->p_mpfinib = (const MPFINIB *) p_mpfinib;
	queue_insert_prev(&(p_dominib->p_domcb->free_mpfcb),
										&(p_mpfcb->wait_queue));
}

void
initialize_mempfix(void)
{
	uint_t			i, j, k;
	ID				domid;
	MPFCB			*p_mpfcb;
	const DOMINIB	*p_dominib;

	for (i = 0; i < tnum_smpf; i++) {
		p_mpfcb = &(mpfcb_table[i]);
		queue_initialize(&(p_mpfcb->wait_queue));
		p_mpfcb->p_mpfinib = &(mpfinib_table[i]);
		p_mpfcb->fblkcnt = p_mpfcb->p_mpfinib->blkcnt;
		p_mpfcb->unused = 0U;
		p_mpfcb->freelist = INDEX_NULL;
	}

	queue_initialize(&(dominib_kernel.p_domcb->free_mpfcb));
	for (j = 0; j < dominib_kernel.tnum_ampfid; i++, j++) {
		initialize_mpfcb(&(mpfcb_table[i]), &(ampfinib_table[j]),
													&dominib_kernel);
	}
	for (domid = TMIN_DOMID; domid <= tmax_domid; domid++) {
		p_dominib = get_dominib(domid);
		queue_initialize(&(p_dominib->p_domcb->free_mpfcb));
		for (k = 0; k < p_dominib->tnum_ampfid; i++, j++, k++) {
			initialize_mpfcb(&(mpfcb_table[i]), &(ampfinib_table[j]),
															p_dominib);
		}
	}
	for (j = 0; j < dominib_none.tnum_ampfid; i++, j++) {
		initialize_mpfcb(&(mpfcb_table[i]), &(ampfinib_table[j]),
													&dominib_none);
	}
}

#endif /* TOPPERS_mpfini */

/*
 *  固定長メモリプールからブロックを獲得
 */
#ifdef TOPPERS_mpfget

void
get_mpf_block(MPFCB *p_mpfcb, void **p_blk)
{
	uint_t	blkidx;

	if (p_mpfcb->freelist != INDEX_NULL) {
		blkidx = p_mpfcb->freelist;
		p_mpfcb->freelist = (p_mpfcb->p_mpfinib->p_mpfmb + blkidx)->next;
	}
	else {
		blkidx = p_mpfcb->unused;
		p_mpfcb->unused++;
	}
	*p_blk = (void *)((char *)(p_mpfcb->p_mpfinib->mpf)
								+ p_mpfcb->p_mpfinib->blksz * blkidx);
	p_mpfcb->fblkcnt--;
	(p_mpfcb->p_mpfinib->p_mpfmb + blkidx)->next = INDEX_ALLOC;
}

#endif /* TOPPERS_mpfget */

/*
 *  固定長メモリプールの生成
 */
#ifdef TOPPERS_acre_mpf

ER_UINT
acre_mpf(const T_CMPF *pk_cmpf)
{
	MPFCB			*p_mpfcb;
	MPFINIB			*p_mpfinib;
	ATR				mpfatr;
	uint_t			blkcnt, blksz;
	MPF_T			*mpf;
	void			*p_mpfmb;
	ID				domid;
	const DOMINIB	*p_dominib;
	ACPTN			acptn;
	ER				ercd;

	LOG_ACRE_MPF_ENTER(pk_cmpf);
	CHECK_TSKCTX_UNL();
	CHECK_MACV_READ(pk_cmpf, T_CMPF);

	mpfatr = pk_cmpf->mpfatr;
	blkcnt = pk_cmpf->blkcnt;
	blksz = pk_cmpf->blksz;
	mpf = pk_cmpf->mpf;
	p_mpfmb = pk_cmpf->mpfmb;

	CHECK_VALIDATR(mpfatr, TA_TPRI|TA_DOMMASK);
	CHECK_PAR(blkcnt != 0);
	CHECK_PAR(blksz != 0);
	CHECK_NOSPT(mpf != NULL);
	CHECK_PAR(MPF_ALIGN(mpf));
	CHECK_OBJ(valid_memobj(mpf, ROUND_MPF_T(blksz) * blkcnt));
	if (p_mpfmb != NULL) {
		CHECK_PAR(MB_ALIGN(p_mpfmb));
		CHECK_OBJ(valid_memobj_kernel(p_mpfmb, sizeof(MPFMB) * blkcnt));
	}
	domid = get_atrdomid(mpfatr);
	if (domid == TDOM_SELF) {
		if (rundom == TACP_KERNEL) {
			domid = TDOM_KERNEL;
		}
		else {
			domid = p_runtsk->p_tinib->domid;
		}
		mpfatr = set_atrdomid(mpfatr, domid);
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
	if (tnum_mpf == 0 || queue_empty(&(p_dominib->p_domcb->free_mpfcb))) {
		ercd = E_NOID;
	}
	else {
		if (p_mpfmb == NULL) {
			p_mpfmb = malloc_mpk(sizeof(MPFMB) * blkcnt, p_dominib);
			mpfatr |= TA_MBALLOC;
		}
		if (p_mpfmb == NULL) {
			ercd = E_NOMEM;
		}
		else {
			p_mpfcb = (MPFCB *)
						queue_delete_next(&(p_dominib->p_domcb->free_mpfcb));
			p_mpfinib = (MPFINIB *)(p_mpfcb->p_mpfinib);
			p_mpfinib->mpfatr = mpfatr;
			p_mpfinib->blkcnt = blkcnt;
			p_mpfinib->blksz = ROUND_MPF_T(blksz);
			p_mpfinib->mpf = mpf;
			p_mpfinib->p_mpfmb = p_mpfmb;

			acptn = default_acptn(domid);
			p_mpfinib->acvct.acptn1 = acptn;
			p_mpfinib->acvct.acptn2 = acptn;
			p_mpfinib->acvct.acptn3 = p_dominib->acvct.acptn1;
			p_mpfinib->acvct.acptn4 = acptn;

			queue_initialize(&(p_mpfcb->wait_queue));
			p_mpfcb->fblkcnt = blkcnt;
			p_mpfcb->unused = 0U;
			p_mpfcb->freelist = INDEX_NULL;
			ercd = MPFID(p_mpfcb);
		}
	}
	unlock_cpu();

  error_exit:
	LOG_ACRE_MPF_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_acre_mpf */

/*
 *  固定長メモリプールのアクセス許可ベクタの設定
 */
#ifdef TOPPERS_sac_mpf

ER
sac_mpf(ID mpfid, const ACVCT *p_acvct)
{
	MPFCB	*p_mpfcb;
	MPFINIB	*p_mpfinib;
	ER		ercd;

	LOG_SAC_MPF_ENTER(mpfid, p_acvct);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_MPFID(mpfid));
	CHECK_MACV_READ(p_acvct, ACVCT);
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (MPFID(p_mpfcb) <= tmax_smpfid) {
		ercd = E_OBJ;
	}
	else {
		p_mpfinib = (MPFINIB *)(p_mpfcb->p_mpfinib);
		p_mpfinib->acvct = *p_acvct;
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_SAC_MPF_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_sac_mpf */

/*
 *  固定長メモリプールの削除
 */
#ifdef TOPPERS_del_mpf

ER
del_mpf(ID mpfid)
{
	MPFCB			*p_mpfcb;
	MPFINIB			*p_mpfinib;
	const DOMINIB	*p_dominib;
	ER				ercd;

	LOG_DEL_MPF_ENTER(mpfid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_MPFID(mpfid));
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else if (MPFID(p_mpfcb) <= tmax_smpfid) {
		ercd = E_OBJ;
	}
	else {
		init_wait_queue(&(p_mpfcb->wait_queue));
		p_mpfinib = (MPFINIB *)(p_mpfcb->p_mpfinib);
		p_dominib = get_atrdominib(p_mpfinib->mpfatr);
		if ((p_mpfinib->mpfatr & TA_MBALLOC) != 0U) {
			free_mpk(p_mpfinib->p_mpfmb, p_dominib);
		}
		p_mpfinib->mpfatr = TA_NOEXS;
		queue_insert_prev(&(p_dominib->p_domcb->free_mpfcb),
										&(p_mpfcb->wait_queue));
		if (p_runtsk != p_schedtsk) {
			dispatch();
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_DEL_MPF_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_del_mpf */

/*
 *  固定長メモリブロックの獲得
 */
#ifdef TOPPERS_get_mpf

ER
get_mpf(ID mpfid, void **p_blk)
{
	MPFCB		*p_mpfcb;
	WINFO_MPF	winfo_mpf;
	ER			ercd;

	LOG_GET_MPF_ENTER(mpfid, p_blk);
	CHECK_DISPATCH();
	CHECK_ID(VALID_MPFID(mpfid));
	CHECK_MACV_WRITE(p_blk, void *);
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu_dsp();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn1)) {
		ercd = E_OACV;
	}
	else if (p_runtsk->raster) {
		ercd = E_RASTER;
	}
	else if (p_mpfcb->fblkcnt > 0) {
		get_mpf_block(p_mpfcb, p_blk);
		ercd = E_OK;
	}
	else {
		wobj_make_wait((WOBJCB *) p_mpfcb, TS_WAITING_MPF,
											(WINFO_WOBJ *) &winfo_mpf);
		dispatch();
		ercd = winfo_mpf.winfo.wercd;
		if (ercd == E_OK) {
			*p_blk = winfo_mpf.blk;
		}
	}
	unlock_cpu_dsp();

  error_exit:
	LOG_GET_MPF_LEAVE(ercd, p_blk);
	return(ercd);
}

#endif /* TOPPERS_get_mpf */

/*
 *  固定長メモリブロックの獲得（ポーリング）
 */
#ifdef TOPPERS_pget_mpf

ER
pget_mpf(ID mpfid, void **p_blk)
{
	MPFCB	*p_mpfcb;
	ER		ercd;

	LOG_PGET_MPF_ENTER(mpfid, p_blk);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_MPFID(mpfid));
	CHECK_MACV_WRITE(p_blk, void *);
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn1)) {
		ercd = E_OACV;
	}
	else if (p_mpfcb->fblkcnt > 0) {
		get_mpf_block(p_mpfcb, p_blk);
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	unlock_cpu();

  error_exit:
	LOG_PGET_MPF_LEAVE(ercd, p_blk);
	return(ercd);
}

#endif /* TOPPERS_pget_mpf */

/*
 *  固定長メモリブロックの獲得（タイムアウトあり）
 */
#ifdef TOPPERS_tget_mpf

ER
tget_mpf(ID mpfid, void **p_blk, TMO tmout)
{
	MPFCB		*p_mpfcb;
	WINFO_MPF	winfo_mpf;
	TMEVTB		tmevtb;
	ER			ercd;

	LOG_TGET_MPF_ENTER(mpfid, p_blk, tmout);
	CHECK_DISPATCH();
	CHECK_ID(VALID_MPFID(mpfid));
	CHECK_MACV_WRITE(p_blk, void *);
	CHECK_PAR(VALID_TMOUT(tmout));
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu_dsp();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn1)) {
		ercd = E_OACV;
	}
	else if (p_runtsk->raster) {
		ercd = E_RASTER;
	}
	else if (p_mpfcb->fblkcnt > 0) {
		get_mpf_block(p_mpfcb, p_blk);
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		wobj_make_wait_tmout((WOBJCB *) p_mpfcb, TS_WAITING_MPF,
								(WINFO_WOBJ *) &winfo_mpf, &tmevtb, tmout);
		dispatch();
		ercd = winfo_mpf.winfo.wercd;
		if (ercd == E_OK) {
			*p_blk = winfo_mpf.blk;
		}
	}
	unlock_cpu_dsp();

  error_exit:
	LOG_TGET_MPF_LEAVE(ercd, p_blk);
	return(ercd);
}

#endif /* TOPPERS_tget_mpf */

/*
 *  固定長メモリブロックの返却
 */
#ifdef TOPPERS_rel_mpf

ER
rel_mpf(ID mpfid, void *blk)
{
	MPFCB	*p_mpfcb;
	size_t	blkoffset;
	uint_t	blkidx;
	TCB		*p_tcb;
	ER		ercd;
    
	LOG_REL_MPF_ENTER(mpfid, blk);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_MPFID(mpfid));
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn2)) {
		ercd = E_OACV;
	}
	else {
		blkoffset = ((char *) blk) - (char *)(p_mpfcb->p_mpfinib->mpf);
		blkidx = (uint_t)(blkoffset / p_mpfcb->p_mpfinib->blksz);
		if (!(p_mpfcb->p_mpfinib->mpf <= blk)
				|| !(blkoffset % p_mpfcb->p_mpfinib->blksz == 0U)
				|| !(blkoffset / p_mpfcb->p_mpfinib->blksz < p_mpfcb->unused)
				|| !((p_mpfcb->p_mpfinib->p_mpfmb + blkidx)->next
															== INDEX_ALLOC)) {
			ercd = E_PAR;
		}
		else if (!queue_empty(&(p_mpfcb->wait_queue))) {
			p_tcb = (TCB *) queue_delete_next(&(p_mpfcb->wait_queue));
			((WINFO_MPF *)(p_tcb->p_winfo))->blk = blk;
			wait_complete(p_tcb);
			if (p_runtsk != p_schedtsk) {
				dispatch();
			}
			ercd = E_OK;
		}
		else {
			p_mpfcb->fblkcnt++;
			(p_mpfcb->p_mpfinib->p_mpfmb + blkidx)->next = p_mpfcb->freelist;
			p_mpfcb->freelist = blkidx;
			ercd = E_OK;
		}
	}
	unlock_cpu();

  error_exit:
	LOG_REL_MPF_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_rel_mpf */

/*
 *  固定長メモリプールの再初期化
 */
#ifdef TOPPERS_ini_mpf

ER
ini_mpf(ID mpfid)
{
	MPFCB	*p_mpfcb;
	ER		ercd;
    
	LOG_INI_MPF_ENTER(mpfid);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_MPFID(mpfid));
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn3)) {
		ercd = E_OACV;
	}
	else {
		init_wait_queue(&(p_mpfcb->wait_queue));
		p_mpfcb->fblkcnt = p_mpfcb->p_mpfinib->blkcnt;
		p_mpfcb->unused = 0U;
		p_mpfcb->freelist = INDEX_NULL;
		if (p_runtsk != p_schedtsk) {
			dispatch();
		}
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_INI_MPF_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ini_mpf */

/*
 *  固定長メモリプールの状態参照
 */
#ifdef TOPPERS_ref_mpf

ER
ref_mpf(ID mpfid, T_RMPF *pk_rmpf)
{
	MPFCB	*p_mpfcb;
	ER		ercd;
    
	LOG_REF_MPF_ENTER(mpfid, pk_rmpf);
	CHECK_TSKCTX_UNL();
	CHECK_ID(VALID_MPFID(mpfid));
	CHECK_MACV_WRITE(pk_rmpf, T_RMPF);
	p_mpfcb = get_mpfcb(mpfid);

	lock_cpu();
	if (p_mpfcb->p_mpfinib->mpfatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (VIOLATE_ACPTN(p_mpfcb->p_mpfinib->acvct.acptn4)) {
		ercd = E_OACV;
	}
	else {
		pk_rmpf->wtskid = wait_tskid(&(p_mpfcb->wait_queue));
		pk_rmpf->fblkcnt = p_mpfcb->fblkcnt;
		ercd = E_OK;
	}
	unlock_cpu();

  error_exit:
	LOG_REF_MPF_LEAVE(ercd, pk_rmpf);
	return(ercd);
}

#endif /* TOPPERS_ref_mpf */
