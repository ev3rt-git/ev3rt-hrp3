/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2018 by Embedded and Real-Time Systems Laboratory
 *                  Graduate School of Informatics, Nagoya Univ., JAPAN
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
 *  $Id: test_prbstr.c 667 2019-02-13 07:42:11Z ertl-hiro $
 */

/* 
 *		prb_strのテスト
 *
 * 【テストの目的】
 *
 *  prb_strの分岐カバレッジが100%になるようにテストする．
 *
 * 【テスト項目】
 *
 *	(A) prb_strの分岐カバレッジ網羅（prb_strのソースコードを参照）
 *		(A-1) ref_memがエラーになる場合
 *		(A-2) limitlen < maxlenの場合
 *		(A-3) limitlen < maxlenでない場合
 *		(A-4) len < maxlenの場合
 *		(A-5) len < maxlenでない場合
 *
 * 【使用リソース】
 *
 *	TASK1: 中優先度タスク，最初から起動，ユーザドメイン
 *	EXTSVC1: 拡張サービスコール
 *	var1[PAGE_SIZE]: ユーザドメインのリード／ライト領域においた配列
 *	var2[PAGE_SIZE]: ユーザドメインのリードオンリー領域においた配列
 *
 * 【テストシーケンス】
 *
 *	== TASK1（優先度：中）==
 *	1:	cal_svc(TFN_EXTSVC1, 0, 0, 0, 0, 0)
 *	== EXTSVC1 ==
 *	2:	DO(memset(var1, '@', PAGE_SIZE))
 *		DO(var1[15] = '\0')
 *	3:	ref_mem(var1, &rmem)
 *		assert(rmem.accatr == (TA_BSSSEC & ~TA_MEMZERO)
 *		assert(rmem.base == ((void *) var1))
 *		assert(rmem.size == PAGE_SIZE)
 *	4:	prb_str(var1, 16, TSK_SELF, TPM_WRITE|TPM_READ)			... (A-3)(A-4)
 *	5:	prb_str(var1, 15, \
 *					TSK_SELF, TPM_WRITE|TPM_READ) -> E_OBJ		... (A-3)(A-5)
 *	6:	prb_str(&var1[PAGE_SIZE - 15], 16, \
 *					TSK_SELF, TPM_WRITE|TPM_READ) -> E_OBJ		... (A-2)(A-5)
 *		DO(var1[PAGE_SIZE - 1] = '\0')
 *	7:	prb_str(&var1[PAGE_SIZE - 15], 16, \
 *					TSK_SELF, TPM_WRITE|TPM_READ)				... (A-2)(A-4)
 *	8:	prb_str(var2, 16, \
 *					TSK_SELF, TPM_WRITE|TPM_READ) -> E_MACV		... (A-3)(A-4)
 *	9:	prb_str(var2, 16, TSK_SELF, TPM_READ)					... (A-3)(A-4)
 *	10:	prb_str(var2, 15, TSK_SELF, TPM_READ) -> E_OBJ			... (A-3)(A-5)
 *	11:	prb_str(&var1[PAGE_SIZE * USED_PAGE], 16, \
 *					TSK_SELF, TPM_WRITE|TPM_READ) -> E_NOEXS	... (A-1)
 *		// 上の行でコンパイラが警告（配列の範囲超え）を出す可能性がある．
 *	12: END
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <string.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_prbstr.h"

char		var1[PAGE_SIZE];
const char	var2[PAGE_SIZE] = {
	'@', '@', '@', '@', '@', '@', '@', '@',
	'@', '@', '@', '@', '@', '@', '@', '\0'
};

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

ER_UINT
extsvc1_routine(intptr_t par1, intptr_t par2, intptr_t par3,
								intptr_t par4, intptr_t par5, ID cdmid)
{
	ER_UINT	ercd;
	T_RMEM	rmem;

	check_point(2);
	memset(var1, '@', PAGE_SIZE);

	var1[15] = '\0';

	check_point(3);
	ercd = ref_mem(var1, &rmem);
	check_ercd(ercd, E_OK);

	check_assert(rmem.accatr == (TA_BSSSEC & ~TA_MEMZERO));

	check_assert(rmem.base == ((void *) var1));

	check_assert(rmem.size == PAGE_SIZE);

	check_point(4);
	ercd = prb_str(var1, 16, TSK_SELF, TPM_WRITE|TPM_READ);
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = prb_str(var1, 15, TSK_SELF, TPM_WRITE|TPM_READ);
	check_ercd(ercd, E_OBJ);

	check_point(6);
	ercd = prb_str(&var1[PAGE_SIZE - 15], 16, TSK_SELF, TPM_WRITE|TPM_READ);
	check_ercd(ercd, E_OBJ);

	var1[PAGE_SIZE - 1] = '\0';

	check_point(7);
	ercd = prb_str(&var1[PAGE_SIZE - 15], 16, TSK_SELF, TPM_WRITE|TPM_READ);
	check_ercd(ercd, E_OK);

	check_point(8);
	ercd = prb_str(var2, 16, TSK_SELF, TPM_WRITE|TPM_READ);
	check_ercd(ercd, E_MACV);

	check_point(9);
	ercd = prb_str(var2, 16, TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	check_point(10);
	ercd = prb_str(var2, 15, TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OBJ);

	check_point(11);
	ercd = prb_str(&var1[PAGE_SIZE * USED_PAGE], 16, TSK_SELF, TPM_WRITE|TPM_READ);
	check_ercd(ercd, E_NOEXS);

	check_finish(12);
	check_point(0);
	return(E_SYS);
}

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	ercd = cal_svc(TFN_EXTSVC1, 0, 0, 0, 0, 0);
	check_ercd(ercd, E_OK);

	check_point(0);
}
