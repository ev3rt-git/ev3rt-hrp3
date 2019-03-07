/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
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
 *  $Id: tBitKernel.c 509 2018-10-27 16:22:39Z ertl-hiro $
 */

/*
 *		カーネルの整合性検査
 */

#include "tBitKernel_tecsgen.h"
#include "kernel/kernel_impl.h"
#include "kernel/task.h"
#include "kernel/wait.h"
#include "kernel/semaphore.h"
#include "kernel/eventflag.h"
#include "kernel/dataqueue.h"
#include "kernel/pridataq.h"
#include "kernel/mutex.h"
#include "kernel/messagebuf.h"
#include "kernel/mempfix.h"
#include "kernel/time_event.h"
#include "kernel/check.h"

/*
 *   エラーコードの定義
 */
#define E_SYS_LINENO	ERCD(E_SYS, -(__LINE__))

/*
 *  管理ブロックのアドレスの正当性のチェック
 */
#define VALID_SCHEDCB(p_schedcb) \
		((((char *) p_schedcb) - ((char *) schedcb_table)) \
	 										% sizeof(SCHECB) == 0 \
					&& 0 <= (p_schedcb - schedcb_table) \
					&& (p_schedcb - schedcb_table) < tnum_schedcb

#define VALID_TCB(p_tcb) \
		((((char *) p_tcb) - ((char *) tcb_table)) % sizeof(TCB) == 0 \
			&& TMIN_TSKID <= TSKID(p_tcb) && TSKID(p_tcb) <= tmax_tskid)

#define VALID_SEMCB(p_semcb) \
		((((char *) p_semcb) - ((char *) semcb_table)) % sizeof(SEMCB) == 0 \
			&& TMIN_SEMID <= SEMID(p_semcb) && SEMID(p_semcb) <= tmax_semid)
				
#define VALID_FLGCB(p_flgcb) \
		((((char *) p_flgcb) - ((char *) flgcb_table)) % sizeof(FLGCB) == 0 \
			&& TMIN_FLGID <= FLGID(p_flgcb) && FLGID(p_flgcb) <= tmax_flgid)

#define VALID_DTQCB(p_dtqcb) \
		((((char *) p_dtqcb) - ((char *) dtqcb_table)) % sizeof(DTQCB) == 0 \
			&& TMIN_DTQID <= DTQID(p_dtqcb) && DTQID(p_dtqcb) <= tmax_dtqid)

#define VALID_PDQCB(p_pdqcb) \
		((((char *) p_pdqcb) - ((char *) pdqcb_table)) % sizeof(PDQCB) == 0 \
			&& TMIN_PDQID <= PDQID(p_pdqcb) && PDQID(p_pdqcb) <= tmax_pdqid)

#define VALID_MTXCB(p_mtxcb) \
		((((char *) p_mtxcb) - ((char *) mtxcb_table)) % sizeof(MTXCB) == 0 \
			&& TMIN_MTXID <= MTXID(p_mtxcb) && MTXID(p_mtxcb) <= tmax_mtxid)

#define VALID_MBFCB(p_mbfcb) \
		((((char *) p_mbfcb) - ((char *) mbfcb_table)) % sizeof(MBFCB) == 0 \
			&& TMIN_MBFID <= MBFID(p_mbfcb) && MBFID(p_mbfcb) <= tmax_mbfid)

#define VALID_MPFCB(p_mpfcb) \
		((((char *) p_mpfcb) - ((char *) mpfcb_table)) % sizeof(MPFCB) == 0 \
			&& TMIN_MPFID <= MPFID(p_mpfcb) && MPFID(p_mpfcb) <= tmax_mpfid)

/*
 *  初期化ブロックのアドレスの正当性のチェック
 */
#define VALID_TWDINIB(p_twdinib) \
		((((char *) p_twdinib) - ((char *) twdinib_table)) \
				 										% sizeof(TWDINIB) == 0

#define VALID_SOMINIB(p_sominib) \
		((((char *) p_sominib) - ((char *) sominib_table)) \
				 										% sizeof(SOMINIB) == 0 \
			&& TMIN_SOMID <= SOMID(p_sominib) && SOMID(p_sominib) <= tmax_somid)

/*
 *  キューのチェックのための関数
 *
 *  p_queueにp_entryが含まれているかを調べる．含まれていればtrue，含ま
 *  れていない場合にはfalseを返す．ダブルリンクの不整合の場合にも，
 *  falseを返す．
 */
static bool_t
in_queue(QUEUE *p_queue, QUEUE *p_entry)
{
	QUEUE	*p_current, *p_next;

	p_current = p_queue->p_next;
	if (p_current->p_prev != p_queue) {
		return(false);					/* ダブルリンクの不整合 */
	}
	while (p_current != p_queue) {
		if (p_current == p_entry) {
			return(true);				/* p_entryが含まれていた */
		}

		/*
		 *  キューの次の要素に進む
		 */
		p_next = p_current->p_next;
		if (p_next->p_prev != p_current) {
			return(false);				 /* ダブルリンクの不整合 */
		}
		p_current = p_next;
	}
	return(false);
}

/*
 *  スタック上を指しているかの検査
 */
static bool_t
on_stack(void *addr, const TINIB *p_tinib)
{
#ifdef USE_TSKINICTXB
	/*
	 *  この関数の実装はターゲット依存になる．
	 */
	return(true);
#else /* USE_TSKINICTXB */
	if (p_tinib->sstk <= addr
			&& addr < (void *)((char *)(p_tinib->sstk) + p_tinib->sstksz)) {
		return(true);
	}
	return(false);
#endif /* USE_TSKINICTXB */
}

/*
 *  スケジューリング単位管理ブロックの検査
 */
#ifndef PRIMAP_BIT
#define	PRIMAP_BIT(pri)		(1U << (pri))
#endif /* PRIMAP_BIT */

Inline bool_t
primap_empty(SCHEDCB *p_schedcb)
{
#ifndef PRIMAP_LEVEL_2
	return(p_schedcb->ready_primap == 0U);
#else /* PRIMAP_LEVEL_2 */
	return(p_schedcb->ready_primap1 == 0U);
#endif /* PRIMAP_LEVEL_2 */
}

Inline uint16_t
primap_extract_bit(uint_t pri, SCHEDCB *p_schedcb)
{
#ifndef PRIMAP_LEVEL_2
	return(p_schedcb->ready_primap & PRIMAP_BIT(pri));
#else /* PRIMAP_LEVEL_2 */
	return(p_schedcb->ready_primap2[pri / TBIT_PRIMAP]
									& PRIMAP_BIT(pri % TBIT_PRIMAP));
#endif /* PRIMAP_LEVEL_2 */
}

static ER
bit_schedcb(SCHEDCB *p_schedcb)
{
	uint_t	pri;
	QUEUE	*p_queue;
	TCB		*p_tcb;

	/*
	 *  p_predtskの整合性の検査
	 */
	if (primap_empty(p_schedcb)) {
		if (p_schedcb->p_predtsk != NULL) {
			return(E_SYS_LINENO);
		}
	}
	else {
		if (p_schedcb->p_predtsk != search_predtsk(p_schedcb)) {
			return(E_SYS_LINENO);
		}
	}

#ifdef PRIMAP_LEVEL_2
	/*
	 *  ready_primap1とready_primap2の整合性の検査
	 */
	for (pri = 0; pri < TNUM_TPRI; pri += TBIT_PRIMAP) {
		if (p_schedcb->ready_primap2[pri / TBIT_PRIMAP] == 0U) {
			if ((p_schedcb->ready_primap1 & PRIMAP_BIT(pri / TBIT_PRIMAP))
																	!= 0U) {
				return(E_SYS_LINENO);
			}
		}
		else {
			if ((p_schedcb->ready_primap1 & PRIMAP_BIT(pri / TBIT_PRIMAP))
																	== 0U) {
				return(E_SYS_LINENO);
			}
		}
	}
#endif /* PRIMAP_LEVEL_2 */

	/*
	 *  ready_queueとready_primapの整合性の検査
	 */
	for (pri = 0; pri < TNUM_TPRI; pri++) {
		p_queue = p_schedcb->ready_queue[pri].p_next;
		if (p_queue == &(p_schedcb->ready_queue[pri])) {
			if (primap_extract_bit(pri, p_schedcb) != 0U) {
				return(E_SYS_LINENO);
			}
		}
		else {
			if (primap_extract_bit(pri, p_schedcb) == 0U) {
				return(E_SYS_LINENO);
			}
		}
		while (p_queue != &(p_schedcb->ready_queue[pri])) {
			p_tcb = (TCB *) p_queue;
			if (!VALID_TCB(p_tcb)) {
				return(E_SYS_LINENO);
			}
			if (!TSTAT_RUNNABLE(p_tcb->tstat)) {
				return(E_SYS_LINENO);
			}
			if (p_tcb->priority != pri) {
				return(E_SYS_LINENO);
			}
			p_queue = p_queue->p_next;
		}
	}
	return(E_OK);
}

/*
 *  保護ドメイン関連の整合性検査
 */
static ER
bit_domain(void)
{
	uint_t	i;
	ER		ercd;

	/*
	 *  p_schedtskの整合性検査
	 */
	if (dspflg) {
		if (schedcb_kernel.p_predtsk != NULL) {
			if (p_twdsched->p_predtsk != NULL
					&& p_twdsched->p_predtsk->priority
								< schedcb_kernel.p_predtsk->priority) {
				if (p_schedtsk != p_twdsched->p_predtsk) {
					return(E_SYS_LINENO);
				}
			}
			else {
				if (p_schedtsk != schedcb_kernel.p_predtsk) {
					return(E_SYS_LINENO);
				}
			}
		}
		else {
			if (p_twdsched->p_predtsk != NULL) {
				if (p_schedtsk != p_twdsched->p_predtsk) {
					return(E_SYS_LINENO);
				}
			}
			else {
				if (p_schedtsk != p_idlesched->p_predtsk) {
					return(E_SYS_LINENO);
				}
			}
		}
	}

	/*
	 *  スケジューリング単位毎の整合性検査
	 */
	ercd = bit_schedcb(&schedcb_kernel);
	if (ercd != E_OK) {
		return(ercd);
	}
	ercd = bit_schedcb(&schedcb_idle);
	if (ercd != E_OK) {
		return(ercd);
	}
	for (i = 0; i < tnum_schedcb; i++) {
		ercd = bit_schedcb(&(schedcb_table[i]));
		if (ercd != E_OK) {
			return(ercd);
		}
	}

	/*
	 *  p_cursom，p_runtwd，p_twdsched，p_idleschedの整合性検査
	 */
	if (system_cyctim == 0U) {
		/*
		 *  時間パーティショニングを使用しない場合の検査
		 */
		if (p_cursom != NULL) {
			return(E_SYS_LINENO);
		}
		if (p_runtwd != NULL) {
			return(E_SYS_LINENO);
		}
		if (p_twdsched != &schedcb_idle) {
			return(E_SYS_LINENO);
		}
		if (p_idlesched != &schedcb_idle) {
			return(E_SYS_LINENO);
		}
	}
	else if (p_cursom == NULL) {
		/*
		 *  システム周期停止モードの場合の検査
		 */
		if (p_runtwd != NULL) {
			return(E_SYS_LINENO);
		}
		if (p_twdsched != &schedcb_kernel) {
			return(E_SYS_LINENO);
		}
		if (p_idlesched != &schedcb_kernel) {
			return(E_SYS_LINENO);
		}
	}
	else {
		/*
		 *  システム周期停止モードでない場合の検査
		 */
		if (!VALID_SOMINIB(p_cursom)) {
			return(E_SYS_LINENO);
		}

		if (p_runtwd == NULL) {
			/*
			 *  アイドルウィンドウの場合の検査
			 */
			if (p_twdsched != &schedcb_idle) {
				return(E_SYS_LINENO);
			}
			if (p_idlesched != &schedcb_idle) {
				return(E_SYS_LINENO);
			}
		}
		else {
			/*
			 *  アイドルウィンドウでない場合の検査
			 */
			if (!(VALID_TWDINIB(p_runtwd) && p_cursom->p_twdinib <= p_runtwd
									&& p_runtwd < (p_cursom + 1)->p_twdinib))) {
				return(E_SYS_LINENO);
			}
			if (p_twdsched != p_runtwd->p_dominib->p_schedcb) {
				return(E_SYS_LINENO);
			}
			if (p_idlesched != &schedcb_idle) {
				return(E_SYS_LINENO);
			}
		}
	}

	return(E_OK);
}

/*
 *  タスク毎の整合性検査
 */
static ER
bit_task(ID tskid)
{
	TCB				*p_tcb;
	const TINIB		*p_tinib;
	const DOMINIB	*p_dominib;
	SCHEDCB			*p_schedcb;
	uint_t			tstat, bpri, pri;
	TMEVTB			*p_tmevtb;
	SEMCB			*p_semcb;
	FLGCB			*p_flgcb;
	DTQCB			*p_dtqcb;
	PDQCB			*p_pdqcb;
	MTXCB			*p_mtxcb;
	MBFCB			*p_mbfcb;
	MPFCB			*p_mpfcb;

	if (!VALID_TSKID(tskid)) {
		return(E_ID);
	}
	p_tcb = get_tcb(tskid);
	p_tinib = p_tcb->p_tinib;
	p_dominib = p_tcb->p_dominib;
	p_schedcb = p_tcb->p_schedcb;
	tstat = p_tcb->tstat;
	bpri = p_tcb->bpriority;
	pri = p_tcb->priority;

	/*
	 *  タスク初期化ブロックへのポインタの検査
	 */
	if (p_tinib != &(tinib_table[INDEX_TSK(tskid)])) {
		return(E_SYS_LINENO);
	}

	/*
	 *  保護ドメイン初期化ブロックとスケジューリング単位管理ブロックへ
	 *  のポインタの検査
	 */
	if (p_tinib->domid == TDOM_KERNEL) {
		if (p_dominib != &dominib_kernel) {
			return(E_SYS_LINENO);
		}
		if (p_schedcb != &schedcb_kernel) {
			return(E_SYS_LINENO);
		}
	}
	else {
		if (p_dominib != get_dominib(p_tinib->domid)) {
			return(E_SYS_LINENO);
		}
		if (p_schedcb != p_dominib->p_schedcb) {
			return(E_SYS_LINENO);
		}
	}

	/*
	 *  tstatの検査
	 */
	if (TSTAT_DORMANT(tstat)) {
		if (tstat != TS_DORMANT) {
			return(E_SYS_LINENO);
		}
	}
	else if (TSTAT_WAITING(tstat)) {
		if ((tstat & ~(TS_WAITING_MASK | TS_SUSPENDED)) != 0U) {
			return(E_SYS_LINENO);
		}
	}
	else if (TSTAT_SUSPENDED(tstat)) {
		if (tstat != TS_SUSPENDED) {
			return(E_SYS_LINENO);
		}
	}
	else {
		if (tstat != TS_RUNNABLE) {
			return(E_SYS_LINENO);
		}
	}

	/*
	 *  拡張サービスコールのネストレベル（svclevel）の検査
	 */
	if (!sense_context() && p_tcb == p_runtsk
										&& p_tinib->domid != TDOM_KERNEL) {
		if (rundom == TACP_KERNEL) {
			if (p_tcb->svclevel == 0) {
				syslog(LOG_NOTICE, "%d %d", TSKID(p_tcb), p_tinib->domid);
				syslog(LOG_NOTICE, "%x %d", rundom, p_tcb->svclevel);
				return(E_SYS_LINENO);
			}
		}
		else {
			if (p_tcb->svclevel > 0) {
				return(E_SYS_LINENO);
			}
		}
	}

	/*
	 *  ベース優先度の検査
	 */
	if (bpri >= TNUM_TPRI) {
		return(E_SYS_LINENO);
	}

	/*
	 *  現在の優先度の検査
	 */
	if (pri > bpri) {
		return(E_SYS_LINENO);
	}

	/*
	 *  rasterと他の状態の整合性の検査
	 */
	if (p_tcb->raster && (p_tcb->enater || TSTAT_WAITING(tstat))) {
		return(E_SYS_LINENO);
	}

	/*
	 *  休止状態におけるチェック
	 */
	if (TSTAT_DORMANT(tstat)) {
		if (!(bpri == p_tinib->ipriority
			  		&& pri == p_tinib->ipriority
					&& p_tcb->actque == false
					&& p_tcb->wupque == false
					&& p_tcb->raster == false
					&& p_tcb->enater == true
					&& p_tcb->p_lastmtx == NULL)) {
			return(E_SYS_LINENO);
		}
	}

	/*
	 *  実行できる状態におけるチェック
	 */
	if (TSTAT_RUNNABLE(tstat)) {
		if (!in_queue(&(p_schedcb->ready_queue[pri]), &(p_tcb->task_queue))) {
			return(E_SYS_LINENO);
		}
	}

	/*
	 *  待ち状態におけるチェック
	 */
	if (TSTAT_WAITING(tstat)) {
		if (!on_stack(p_tcb->p_winfo, p_tinib)) {
			return(E_SYS_LINENO);
		}
		p_tmevtb = p_tcb->p_winfo->p_tmevtb;
		if (p_tmevtb != NULL) {
			if (!on_stack(p_tmevtb, p_tinib)) {
				return(E_SYS_LINENO);
			}
			/*
			 *  (*p_tmevtb)の検査（未完成）
			 */
		}

		switch (tstat & TS_WAITING_MASK) {
		case TS_WAITING_SLP:
			if (p_tcb->wupque == true) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_DLY:
			if (p_tmevtb == NULL) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_SEM:
			p_semcb = ((WINFO_SEM *)(p_tcb->p_winfo))->p_semcb;
			if (!VALID_SEMCB(p_semcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_semcb->wait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_FLG:
			p_flgcb = ((WINFO_FLG *)(p_tcb->p_winfo))->p_flgcb;
			if (!VALID_FLGCB(p_flgcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_flgcb->wait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_SDTQ:
			p_dtqcb = ((WINFO_SDTQ *)(p_tcb->p_winfo))->p_dtqcb;
			if (!VALID_DTQCB(p_dtqcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_dtqcb->swait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_RDTQ:
			p_dtqcb = ((WINFO_RDTQ *)(p_tcb->p_winfo))->p_dtqcb;
			if (!VALID_DTQCB(p_dtqcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_dtqcb->rwait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_SPDQ:
			p_pdqcb = ((WINFO_SPDQ *)(p_tcb->p_winfo))->p_pdqcb;
			if (!VALID_PDQCB(p_pdqcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_pdqcb->swait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_RPDQ:
			p_pdqcb = ((WINFO_RPDQ *)(p_tcb->p_winfo))->p_pdqcb;
			if (!VALID_PDQCB(p_pdqcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_pdqcb->rwait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_MTX:
			p_mtxcb = ((WINFO_MTX *)(p_tcb->p_winfo))->p_mtxcb;
			if (!VALID_MTXCB(p_mtxcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_mtxcb->wait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_SMBF:
			p_mbfcb = ((WINFO_SMBF *)(p_tcb->p_winfo))->p_mbfcb;
			if (!VALID_MBFCB(p_mbfcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_mbfcb->swait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_RMBF:
			p_mbfcb = ((WINFO_RMBF *)(p_tcb->p_winfo))->p_mbfcb;
			if (!VALID_MBFCB(p_mbfcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_mbfcb->rwait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		case TS_WAITING_MPF:
			p_mpfcb = ((WINFO_MPF *)(p_tcb->p_winfo))->p_mpfcb;
			if (!VALID_MPFCB(p_mpfcb)) {
				return(E_SYS_LINENO);
			}
			if (!in_queue(&(p_mpfcb->wait_queue), &(p_tcb->task_queue))) {
				return(E_SYS_LINENO);
			}
			break;

		default:
			return(E_SYS_LINENO);
			break;
		}
	}

	/*
	 *  p_lastmtxの検査
	 */
	if (p_tcb->p_lastmtx != NULL) {
		if (!VALID_MTXCB(p_tcb->p_lastmtx)) {
			return(E_SYS_LINENO);
		}
	}

	/*
	 *  tskctxbの検査
	 */
	if (!TSTAT_DORMANT(tstat) && p_tcb != p_runtsk) {
		/*
		 *  ターゲット依存の検査
		 */
#if 0
		if (bit_tskctxb(&(p_tcb->tskctxb))) {
			return(E_SYS_LINENO);
		}
#endif
	}
	return(E_OK);
}

/*
 *  セマフォ毎の整合性検査
 */
#define INDEX_SEM(semid)	((uint_t)((semid) - TMIN_SEMID))
#define get_semcb(semid)	(&(semcb_table[INDEX_SEM(semid)]))

static ER
bit_semaphore(ID semid)
{
	SEMCB			*p_semcb;
	const SEMINIB	*p_seminib;
	uint_t			semcnt, pri;
	QUEUE			*p_queue;
	TCB				*p_tcb;

	if (!VALID_SEMID(semid)) {
		return(E_ID);
	}
	p_semcb = get_semcb(semid);
	p_seminib = p_semcb->p_seminib;
	semcnt = p_semcb->semcnt;

	/*
	 *  セマフォ初期化ブロックへのポインタの検査
	 */
	if (p_seminib != &(seminib_table[INDEX_SEM(semid)])) {
		return(E_SYS_LINENO);
	}

	/*
	 *  semcntの検査
	 */
	if (semcnt > p_seminib->maxsem) {
		return(E_SYS_LINENO);
	}

	/*
	 *  wait_queueの検査
	 */
	if (semcnt == 0) {
		p_queue = p_semcb->wait_queue.p_next;
		pri = TMIN_TPRI;
		while (p_queue != &(p_semcb->wait_queue)) {
			p_tcb = (TCB *) p_queue;
			if (!VALID_TCB(p_tcb)) {
				return(E_SYS_LINENO);
			}

			/*
			 *  キューがタスク優先度順になっているかの検査
			 */
			if ((p_seminib->sematr & TA_TPRI) != 0U) {
				if (p_tcb->priority < pri) {
					return(E_SYS_LINENO);
				}
			}
			pri = p_tcb->priority;

			/*
			 *  タスク状態の検査
			 */
			if (p_tcb->tstat != TS_WAITING_SEM) {
				return(E_SYS_LINENO);
			}
			if (p_semcb != ((WINFO_SEM *)(p_tcb->p_winfo))->p_semcb) {
				return(E_SYS_LINENO);
			}
			p_queue = p_queue->p_next;
		}
	}
	else {
		if (!queue_empty(&(p_semcb->wait_queue))) {
			return(E_SYS_LINENO);
		}
	}
	return(E_OK);
}

/*
 *  整合性検査関数本体
 */
ER
eBuiltInTest_builtInTest(void)
{
	ID		tskid;
	ID		semid;
	ER		ercd;

	/*
	 *  保護ドメイン関連の整合性検査
	 */
	ercd = bit_domain();
	if (ercd != E_OK) {
		return(ercd);
	}

	/*
	 *  タスク毎の検査
	 */
	for (tskid = TMIN_TSKID; tskid <= tmax_tskid; tskid++) {
		ercd = bit_task(tskid);
		if (ercd != E_OK) {
			return(ercd);
		}
	}

	/*
	 *  セマフォ毎の検査
	 */
	for (semid = TMIN_SEMID; semid <= tmax_semid; semid++) {
		ercd = bit_semaphore(semid);
		if (ercd != E_OK) {
			return(ercd);
		}
	}

	return(E_OK);
}
