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
 *  $Id: test_mprot2.c 597 2018-12-12 00:32:54Z ertl-hiro $
 */

/* 
 *		メモリ保護機能のテスト(2)
 *
 * 【テストの目的】
 *
 *  命令アクセスに対するメモリ保護機能を，prb_memと直接アクセス（実行）
 *  を併用してテストする．
 *
 * 【テスト項目】
 *
 *	(A) 自保護ドメインのテキスト領域
 *		(A-1) 直接アクセス（実行）可能なこと
 *		(A-2) 自タスクからのprb_memが実行可能と判断すること
 *		(A-3) 他タスクからのprb_memが実行可能と判断すること
 *	(B) カーネルドメインのテキスト領域
 *		(B-1) 直接アクセス（実行）不可能なこと
 *		(B-2) 自タスクからのprb_memが実行不可能と判断すること
 *		(B-3) 他タスクからのprb_memが実行不可能と判断すること
 *	(C) 無所属のテキスト領域
 *		(C-1) 直接アクセス（実行）可能なこと
 *		(C-2) 自タスクからのprb_memが実行可能と判断すること
 *		(C-3) 他タスクからのprb_memが実行可能と判断すること
 *	(D) 自保護ドメインのデータ領域
 *		※ ターゲット依存で実行不可にできない場合がある
 *		(D-1) 直接アクセス（実行）不可能なこと
 *		(D-2) 自タスクからのprb_memが実行不可能と判断すること
 *		(D-3) 他タスクからのprb_memが実行不可能と判断すること
 *
 * 【使用リソース】
 *
 *	TASK1: 高優先度タスク，メインタスク，最初から起動，カーネルドメイン
 *	TASK2: 中優先度タスク，最初から起動，ユーザドメイン
 *	TASK3: 最高優先度，例外処理タスク
 *	CPUEXC: メモリ保護違反例外ハンドラ
 *	func1: 自保護ドメインのテキスト領域においた関数
 *	func2: カーネルドメインのテキスト領域においた関数
 *	func3: 無所属のテキスト領域においた関数
 *	func4: 自保護ドメインのデータ領域においた関数
 *
 * 【テストシーケンス】
 *
 *	== TASK1（優先度：高）==
 *	1:	DO(var0 = 0)
 *		slp_tsk()
 *	== TASK2-1（1回目，優先度：中）==
 *	2:	DO(func1())												... (A-1)
 *		assert(var0 == MAGIC1)
 *	3:	prb_mem((void *) &func1, 1, TSK_SELF, TPM_EXEC)			... (A-2)
 *	4:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	5:	prb_mem((void *) &func1, 1, TASK2, TPM_EXEC)			... (A-3)
 *		slp_tsk()
 *	== TASK2-1（続き）==
 *	6:	DO(func2())												... (B-1)
 *	== CPUEXC-1 ==
 *  7:	DO(PREPARE_RETURN_MACV_INST)
 *		act_tsk(TASK3)
 *		RETURN
 *	== TASK3-1（優先度：最高）==
 *	8:	ter_tsk(TASK2)
 *		act_tsk(TASK2)
 *		ext_tsk()
 *	== TASK2-2（2回目，優先度：中）==
 *	9:	assert(var0 == MAGIC1)
 *	10:	prb_mem((void *) &func2, 1, TSK_SELF, TPM_EXEC) -> E_MACV	... (B-2)
 *	11:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	12:	prb_mem((void *) &func2, 1, TASK2, TPM_EXEC) -> E_MACV		... (B-3)
 *		slp_tsk()
 *	== TASK2-2（続き）==
 *	13:	DO(func3())												... (C-1)
 *		assert(var0 == MAGIC3)
 *	14:	prb_mem((void *) &func3, 1, TSK_SELF, TPM_EXEC)			... (C-2)
 *	15:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	16:	prb_mem((void *) &func3, 1, TASK2, TPM_EXEC)			... (C-3)
 *		slp_tsk()
 *	== TASK2-2（続き）==
 *	17:	DO(func4())												... (D-1)
 *	== CPUEXC-2 ==
 *  18:	DO(PREPARE_RETURN_MACV_INST)
 *		act_tsk(TASK3)
 *		RETURN
 *	== TASK3-2（2回目，優先度：最高）==
 *	19:	ter_tsk(TASK2)
 *		act_tsk(TASK2)
 *		ext_tsk()
 *	== TASK2-3（3回目，優先度：中）==
 *	20:	assert(var0 == MAGIC3)
 *	21:	prb_mem((void *) &func4, 1, TSK_SELF, TPM_EXEC) -> E_MACV	... (D-2)
 *	22:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	23:	prb_mem((void *) &func4, 1, TASK2, TPM_EXEC) -> E_MACV		... (D-3)
 *	24: END
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

uint_t	var0;

void func1(void) __attribute__((section(".text_DOM1")));
void func2(void) __attribute__((section(".text_kernel_1")));
void func3(void) __attribute__((section(".text_shared_1")));
void func4(void) __attribute__((section(".data_DOM1")));

void
func1(void)
{
	var0 = MAGIC1;
}

void
func2(void)
{
	var0 = MAGIC2;

#ifdef TOPPERS_ARM
	/* ARM MMUでは，これがないとうまくいかない．原因不明 */
	Asm("nop; nop; nop; nop");
	Asm("nop; nop; nop; nop");
	Asm("nop; nop; nop; nop");
	Asm("nop; nop; nop; nop");
#endif /* TOPPERS_ARM */
}

void
func3(void)
{
	var0 = MAGIC3;
}

void
func4(void)
{
	var0 = MAGIC4;

#ifdef TOPPERS_ARM
	/* ARM MMUでは，これがないとうまくいかない．原因不明 */
	Asm("nop; nop; nop; nop");
	Asm("nop; nop; nop; nop");
	Asm("nop; nop; nop; nop");
	Asm("nop; nop; nop; nop");
#endif /* TOPPERS_ARM */
}

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

static uint_t	cpuexc_count = 0;
extern void _kernel_default_exc_handler(void *p_excinf, EXCNO excno);

void
cpuexc_handler(void *p_excinf)
{
	ER_UINT	ercd;

	switch (++cpuexc_count) {
	case 1:
		check_point(7);
		PREPARE_RETURN_MACV_INST;

		ercd = act_tsk(TASK3);
		check_ercd(ercd, E_OK);

		return;

		check_point(0);

	case 2:
		check_point(18);
		PREPARE_RETURN_MACV_INST;

		ercd = act_tsk(TASK3);
		check_ercd(ercd, E_OK);

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
	var0 = 0;

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = prb_mem((void *) &func1, 1, TASK2, TPM_EXEC);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(12);
	ercd = prb_mem((void *) &func2, 1, TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(16);
	ercd = prb_mem((void *) &func3, 1, TASK2, TPM_EXEC);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(23);
	ercd = prb_mem((void *) &func4, 1, TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_finish(24);
	check_point(0);
}

static uint_t	task2_count = 0;

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++task2_count) {
	case 1:
		check_point(2);
		func1();

		check_assert(var0 == MAGIC1);

		check_point(3);
		ercd = prb_mem((void *) &func1, 1, TSK_SELF, TPM_EXEC);
		check_ercd(ercd, E_OK);

		check_point(4);
		ercd = wup_tsk(TASK1);
		check_ercd(ercd, E_OK);

		check_point(6);
		func2();

		check_point(0);

	case 2:
		check_point(9);
		check_assert(var0 == MAGIC1);

		check_point(10);
		ercd = prb_mem((void *) &func2, 1, TSK_SELF, TPM_EXEC);
		check_ercd(ercd, E_MACV);

		check_point(11);
		ercd = wup_tsk(TASK1);
		check_ercd(ercd, E_OK);

		check_point(13);
		func3();

		check_assert(var0 == MAGIC3);

		check_point(14);
		ercd = prb_mem((void *) &func3, 1, TSK_SELF, TPM_EXEC);
		check_ercd(ercd, E_OK);

		check_point(15);
		ercd = wup_tsk(TASK1);
		check_ercd(ercd, E_OK);

		check_point(17);
		func4();

		check_point(0);

	case 3:
		check_point(20);
		check_assert(var0 == MAGIC3);

		check_point(21);
		ercd = prb_mem((void *) &func4, 1, TSK_SELF, TPM_EXEC);
		check_ercd(ercd, E_MACV);

		check_point(22);
		ercd = wup_tsk(TASK1);
		check_ercd(ercd, E_OK);

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}

static uint_t	task3_count = 0;

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++task3_count) {
	case 1:
		check_point(8);
		ercd = ter_tsk(TASK2);
		check_ercd(ercd, E_OK);

		ercd = act_tsk(TASK2);
		check_ercd(ercd, E_OK);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 2:
		check_point(19);
		ercd = ter_tsk(TASK2);
		check_ercd(ercd, E_OK);

		ercd = act_tsk(TASK2);
		check_ercd(ercd, E_OK);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}
