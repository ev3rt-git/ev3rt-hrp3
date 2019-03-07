/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2015-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_mprot1.c 370 2018-04-15 05:43:33Z ertl-hiro $
 */

/* 
 *		メモリ保護機能のテスト(1)
 *
 * 【テストの目的】
 *
 *  データアクセスに対するメモリ保護機能を，prb_memと直接アクセスを併用
 *  してテストする．
 *
 * 【テスト項目】
 *
 *	(A) 自保護ドメインのリード／ライト領域
 *		(A-1) 直接アクセスによりリードが可能なこと
 *		(A-2) 自タスクからのprb_memがリード可能と判断すること
 *		(A-3) 他タスクからのprb_memがリード可能と判断すること
 *		(A-4) 直接アクセスによりライトが可能なこと
 *		(A-5) 自タスクからのprb_memがライト可能と判断すること
 *		(A-6) 他タスクからのprb_memがライト可能と判断すること
 *	(B) カーネルドメインの共有リード／専有ライト領域
 *		(B-1) 直接アクセスによりリードが可能なこと
 *		(B-2) 自タスクからのprb_memがリード可能と判断すること
 *		(B-3) 他タスクからのprb_memがリード可能と判断すること
 *		(B-4) 直接アクセスによりライトが不可能なこと
 *		(B-5) 自タスクからのprb_memがライト不可能と判断すること
 *		(B-6) 他タスクからのprb_memがライト不可能と判断すること
 *	(C) カーネルドメインの専有リード／ライト領域
 *		(C-1) 直接アクセスによりリードが不可能なこと
 *		(C-2) 自タスクからのprb_memがリード不可能と判断すること
 *		(C-3) 他タスクからのprb_memがリード不可能と判断すること
 *		(C-4) 直接アクセスによりライトが不可能なこと
 *		(C-5) 自タスクからのprb_memがライト不可能と判断すること
 *		(C-6) 他タスクからのprb_memがライト不可能と判断すること
 *
 * 【使用リソース】
 *
 *	TASK1: 高優先度タスク，メインタスク，最初から起動，カーネルドメイン
 *	TASK2: 中優先度タスク，最初から起動，ユーザドメイン
 *	CPUEXC: メモリ保護違反例外ハンドラ
 *	var1: 自保護ドメインのリード／ライト領域においた変数
 *	var2: カーネルドメインの共有リード／専有ライト領域においた変数
 *	var3: カーネルドメインの専有リード／ライト領域においた変数
 *
 * 【テストシーケンス】
 *
 *	== TASK1（優先度：高）==
 *	1:	DO(var1 = MAGIC1)
 *		slp_tsk()
 *	== TASK2（優先度：中）==
 *	2:	DO(var0 = var1)												... (A-1)
 *		assert(var0 == MAGIC1)
 *	3:	prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_READ)
 *																	... (A-2)
 *	4:	DO(var1 = MAGIC2)											... (A-4)
 *	5:	prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_WRITE)
 *																	... (A-5)
 *		prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *	6:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	7:	assert(var1 == MAGIC2)
 *	8:	prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_READ)	... (A-3)
 *		prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_WRITE)	... (A-6)
 *		prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *	9:	DO(var2 = MAGIC3)
 *		slp_tsk()
 *	== TASK2（続き）==
 *	10:	DO(var0 = var2)												... (B-1)
 *		assert(var0 == MAGIC3)
 *	11:	prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_READ)
 *																	... (B-2)
 *	12:	DO(var2 = MAGIC4)											... (A-4)
 *	== CPUEXC-1 ==
 *  13:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	14:	prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_WRITE) -> E_MACV
 *																	... (B-5)
 *		prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *	15:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	16:	assert(var2 == MAGIC3)
 *	17:	prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_READ)	... (B-3)
 *		prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_WRITE) -> E_MACV
 *																	... (B-6)
 *		prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *	18:	DO(var3 = MAGIC5)
 *		slp_tsk()
 *	== TASK2（続き）==
 *	19:	DO(var1 = var3)												... (C-1)
 *	== CPUEXC-2 ==
 *  20:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	21:	prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_READ) -> E_MACV
 *																	... (C-2)
 *	22:	DO(var3 = MAGIC6)											... (C-4)
 *	== CPUEXC-3 ==
 *  23:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	24:	prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_WRITE) -> E_MACV
 *																	... (C-5)
 *		prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *	25:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	26:	assert(var3 == MAGIC5)
 *	27:	prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_READ) -> E_MACV
 *																	... (C-3)
 *		prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_WRITE) -> E_MACV
 *																	... (C-6)
 *		prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *	28: END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_mprot1.h"

#ifndef PREPARE_RETURN_MACV_INST
#define PREPARE_RETURN_MACV_INST
#endif /* PREPARE_RETURN_MACV_INST */

#ifndef PREPARE_RETURN_MACV_DATA
#define PREPARE_RETURN_MACV_DATA
#endif /* PREPARE_RETURN_MACV_DATA */

#define MAGIC1		0x12345678U
#define MAGIC2		0x23456789U
#define MAGIC3		0x3456789aU
#define MAGIC4		0x456789abU
#define MAGIC5		0x56789abcU
#define MAGIC6		0x6789abcdU

uint_t			var0;
volatile uint_t	var1;
volatile uint_t	var2 __attribute__((section(".srpw_kernel")));
volatile uint_t	var3 __attribute__((section(".data_kernel_1")));

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

static uint_t	cpuexc_count = 0;

void
cpuexc_handler(void *p_excinf)
{

	switch (++cpuexc_count) {
	case 1:
		check_point(13);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_point(0);

	case 2:
		check_point(20);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_point(0);

	case 3:
		check_point(23);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	var1 = MAGIC1;

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(7);
	check_assert(var1 == MAGIC2);

	check_point(8);
	ercd = prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(9);
	var2 = MAGIC3;

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(16);
	check_assert(var2 == MAGIC3);

	check_point(17);
	ercd = prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(18);
	var3 = MAGIC5;

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(26);
	check_assert(var3 == MAGIC5);

	check_point(27);
	ercd = prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_finish(28);
	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(2);
	var0 = var1;

	check_assert(var0 == MAGIC1);

	check_point(3);
	ercd = prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	check_point(4);
	var1 = MAGIC2;

	check_point(5);
	ercd = prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(6);
	ercd = wup_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(10);
	var0 = var2;

	check_assert(var0 == MAGIC3);

	check_point(11);
	ercd = prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	check_point(12);
	var2 = MAGIC4;

	check_point(14);
	ercd = prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(15);
	ercd = wup_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(19);
	var1 = var3;

	check_point(21);
	ercd = prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_MACV);

	check_point(22);
	var3 = MAGIC6;

	check_point(24);
	ercd = prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(25);
	ercd = wup_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(0);
}
