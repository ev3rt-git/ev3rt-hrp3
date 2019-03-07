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
 *  $Id: mem_manage.c 387 2018-04-20 03:32:14Z ertl-hiro $
 */

/*
 *		メモリオブジェクト管理機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "memory.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_PRB_MEM_ENTER
#define LOG_PRB_MEM_ENTER(base, size, tskid, pmmode)
#endif /* LOG_PRB_MEM_ENTER */

#ifndef LOG_PRB_MEM_LEAVE
#define LOG_PRB_MEM_LEAVE(ercd)
#endif /* LOG_PRB_MEM_LEAVE */

#ifndef LOG_REF_MEM_ENTER
#define LOG_REF_MEM_ENTER(addr, pk_rmem)
#endif /* LOG_REF_MEM_ENTER */

#ifndef LOG_REF_MEM_LEAVE
#define LOG_REF_MEM_LEAVE(ercd, pk_rmem)
#endif /* LOG_REF_MEM_LEAVE */

/*
 *  メモリアクセス権のチェック（prb_memの本体）
 */
#ifndef OMIT_PROBE_MEMORY

Inline ER
probe_memory(const void *base, size_t size, TCB *p_tcb, MODE pmmode)
{
	ACPTN	domptn;
	int_t	meminib;
	ATR		accatr;
	size_t	memsize;
	ER		ercd;

	domptn = p_tcb->p_dominib->domptn;
	meminib = search_meminib(base);
	accatr = meminib_table[meminib].accatr;
	memsize = ((char *)((meminib + 1 < tnum_meminib) ?
					memtop_table[meminib + 1] : 0)) - ((char *) base);

	if (accatr == TA_NOEXS) {
		ercd = E_NOEXS;							/*［NGKI2930］*/
	}
	else if (size > memsize) {
		ercd = E_OBJ;							/*［NGKI2932］*/
	}
	else if (rundom != TACP_KERNEL
				&& (rundom & meminib_table[meminib].acptn4) == 0U) {
		ercd = E_OACV;							/*［NGKI2931］*/
	}
	else if (domptn == TACP_KERNEL) {
		/*
		 *  対象タスクがカーネルドメインに属する場合
		 */
		ercd = E_OK;
	}
	else if ((accatr & TA_USTACK) == 0U) {
		/*
		 *  通常のメモリオブジェクト（タスクのユーザスタック領域以外）
		 *  の場合
		 *
		 *  メモリオブジェクト属性がTA_NOWRITEの場合には，acptn1が0Uに
		 *  なっているので，メモリオブジェクト属性のTA_NOWRITEはチェッ
		 *  クしていない．
		 */
		if ((pmmode & TPM_WRITE) != 0U
					&& (domptn & meminib_table[meminib].acptn1) == 0U) {
			ercd = E_MACV;
		}
		else if ((pmmode & TPM_READ) != 0U && ((accatr & TA_NOREAD) != 0U
						|| (domptn & meminib_table[meminib].acptn2) == 0U)) {
			ercd = E_MACV;
		}
		else if ((pmmode & TPM_EXEC) != 0U && ((accatr & TA_EXEC) == 0U
						|| (domptn & meminib_table[meminib].acptn2) == 0U)) {
			ercd = E_MACV;
		}
		else {
			ercd = E_OK;
		}
	}
	else {
		/*
		 *  タスクのユーザスタック領域の場合
		 *
		 *  ユーザスタック領域が読出し禁止や書込み禁止になっていること
		 *  はあり得ないため，メモリオブジェクト属性のTA_NOWRITEと
		 *  TA_NOREADはチェックしていない．
		 */
		if (!within_ustack(base, size, p_tcb)) {
			ercd = E_MACV;
		}
		else if ((pmmode & TPM_EXEC) != 0U && ((accatr & TA_EXEC) == 0U)) {
			ercd = E_MACV;
		}
		else {
			ercd = E_OK;
		}
	}
	return(ercd);
}

#endif /* OMIT_PROBE_MEMORY */

/*
 *  メモリ領域に対するアクセス権のチェック［NGKI2924］
 *
 *  このサービスコールでは，静的なデータ構造しか参照しないため，クリティ
 *  カルセクションを作る必要がない．
 */
#ifdef TOPPERS_prb_mem

ER
prb_mem(const void *base, size_t size, ID tskid, MODE pmmode)
{
	TCB		*p_tcb;
	ER		ercd;

	LOG_PRB_MEM_ENTER(base, size, tskid, pmmode);
	CHECK_TSKCTX();								/*［NGKI2925］*/
	CHECK_PAR(size > 0U);						/*［NGKI2929］*/
	if (tskid == TSK_SELF) {
		p_tcb = p_runtsk;						/*［NGKI2939］*/
	}
	else {
		CHECK_ID(VALID_TSKID(tskid));			/*［NGKI2927］*/
		p_tcb = get_tcb(tskid);
	}
	CHECK_PAR((pmmode & ~(TPM_READ|TPM_WRITE|TPM_EXEC)) == 0U);
	CHECK_PAR(pmmode != 0U);					/*［NGKI3438］*/

	lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;							/*［NGKI3425］*/
	}
	else if (VIOLATE_ACPTN(p_tcb->p_tinib->acvct.acptn4)) {
		ercd = E_OACV;							/*［NGKI3426］*/
	}
	else {
		ercd = probe_memory(base, size, p_tcb, pmmode);
	}
	unlock_cpu();

  error_exit:
	LOG_PRB_MEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_prb_mem */

/*
 *  メモリオブジェクトの状態参照［NGKI3954］
 *
 *  このサービスコールでは，静的なデータ構造しか参照しないため，クリティ
 *  カルセクションを作る必要がない．
 */
#ifdef TOPPERS_ref_mem

ER
ref_mem(const void *addr, T_RMEM *pk_rmem)
{
	int_t	meminib;
	ATR		accatr;
	ER		ercd;

	LOG_REF_MEM_ENTER(base, pk_rmem);
	CHECK_TSKCTX();								/*［NGKI3955］*/
	CHECK_MACV_WRITE(pk_rmem, T_RMEM);			/*［NGKI3958］*/

	meminib = search_meminib(addr);
	accatr = meminib_table[meminib].accatr;
	if (accatr == TA_NOEXS) {
		ercd = E_NOEXS;							/*［NGKI3956］*/
	}
	else if (rundom != TACP_KERNEL
				&& (rundom & meminib_table[meminib].acptn4) == 0U) {
		ercd = E_OACV;							/*［NGKI3957］*/
	}
	else {
		pk_rmem->accatr = accatr;
		pk_rmem->base = memtop_table[meminib];
		pk_rmem->size = ((char *)((meminib + 1 < tnum_meminib) ?
										memtop_table[meminib + 1] : 0))
						- ((char *) memtop_table[meminib]);
		ercd = E_OK;
	}

  error_exit:
	LOG_REF_MEM_LEAVE(ercd, pk_rmem);
	return(ercd);
}

#endif /* TOPPERS_ref_mem */
