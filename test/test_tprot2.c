/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2015-2016 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_tprot2.c 195 2016-04-10 09:30:59Z ertl-hiro $
 */

/* 
 *		時間パーティショニングに関するテスト(2)
 *
 * 【テストの目的】
 *
 *  時間パーティショニングを使用した時のタイムウィンドウ／アイドルウィ
 *  ンドウ内のタスク切換えに関して，様々な状況を網羅的にテストする．
 *
 * 【テスト項目】
 *
 *	(A) タイムウィンドウ内のスケジューリング
 *		(A-1) ユーザドメインのタスク→ユーザドメインのタスク
 *		(A-2) ユーザドメインのタスク→カーネルドメインのタスク
 *		(A-3) ユーザドメインのタスク→アイドルドメインのタスク
 *		(A-4) ユーザドメインのタスク→実行できるタスクなし
 *		(A-5) カーネルドメインのタスク→ユーザドメインのタスク
 *		(A-6) カーネルドメインのタスク→カーネルドメインのタスク
 *		(A-7) カーネルドメインのタスク→アイドルドメインのタスク
 *		(A-8) カーネルドメインのタスク→実行できるタスクなし
 *		(A-9) アイドルドメインのタスク→ユーザドメインのタスク
 *		(A-10) アイドルドメインのタスク→カーネルドメインのタスク
 *		(A-11) アイドルドメインのタスク→アイドルドメインのタスク
 *		(A-12) アイドルドメインのタスク→実行できるタスクなし
 *		(A-13) 実行できるタスクなし→ユーザドメインのタスク
 *		(A-14) 実行できるタスクなし→カーネルドメインのタスク
 *		(A-15) 実行できるタスクなし→アイドルドメインのタスク
 *	(B) アイドルウィンドウ内のスケジューリング
 *		(B-1) カーネルドメインのタスク→カーネルドメインのタスク
 *		(B-2) カーネルドメインのタスク→アイドルドメインのタスク
 *		(B-3) カーネルドメインのタスク→実行できるタスクなし
 *		(B-4) アイドルドメインのタスク→カーネルドメインのタスク
 *		(B-5) アイドルドメインのタスク→アイドルドメインのタスク
 *		(B-6) アイドルドメインのタスク→実行できるタスクなし
 *		(B-7) 実行できるタスクなし→カーネルドメインのタスク
 *		(B-8) 実行できるタスクなし→アイドルドメインのタスク
 *
 * 【使用リソース】
 *
 *	システム周期: (40 * TEST_TIME_CP)
 *	DOM1: システム周期の最初タイムウィンドウ（長さ: 15 * TEST_TIME_CP）
 *		  を割当て
 *	DOM2: システム周期の2番目のタイムウィンドウ（長さ: 5 * TEST_TIME_CP）
 *		  を割当て
 *	DOM3: タイムウィンドウを割り当てない
 *	DOM4: タイムウィンドウを割り当てない
 *	TASK11: DOM1，中優先度，TA_ACT属性
 *	TASK12: DOM1，高優先度
 *	TASK2: DOM2，中優先度，TA_ACT属性
 *	TASK3: DOM3，中優先度，TA_ACT属性
 *	TASK4: DOM4，中優先度
 *	TASK51: カーネルドメイン，中優先度
 *	TASK52: カーネルドメイン，高優先度
 *
 * 【テストシーケンス】
 *
 *	// タイムウィンドウ for DOM1
 *	//		高：なし，中：TASK11，TASK2，TASK3
 *	== TASK11 ==
 *	1:	act_tsk(TASK12)											... (A-1)
 *	//		高：TASK12，中：TASK11，TASK2，TASK3
 *	== TASK12 ==
 *	2:	act_tsk(TASK51)
 *		act_tsk(TASK52)											... (A-2)
 *	//		高：TASK52，TASK12，中：TASK51，TASK11，TASK2，TASK3
 *	== TASK52 ==
 *	3:	sus_tsk(TASK12)
 *		slp_tsk()												... (A-6)
 *	//		高：なし，中：TASK51，TASK11，TASK2，TASK3
 *	== TASK51 ==
 *	4:	rsm_tsk(TASK12)											... (A-5)
 *	//		高：TASK12，中：TASK51，TASK11，TASK2，TASK3
 *	== TASK12 ==
 *	5:	slp_tsk()												... (A-2)
 *	== TASK51 ==
 *	6:	slp_tsk()												... (A-5)
 *	== TASK11 ==
 *	7:	slp_tsk()												... (A-3)
 *	//		高：なし，中：TASK2，TASK3
 *	== TASK3 ==
 *	8:	act_tsk(TASK4)
 *		rot_rdq(TPRI_SELF)										... (A-11)
 *	//		高：なし，中：TASK2，TASK4→TASK3
 *	== TASK4 ==
 *	9:	wup_tsk(TASK51)											... (A-10)
 *	//		高：なし，中：TASK51，TASK2，TASK4→TASK3
 *	== TASK51 ==
 *	10:	slp_tsk()												... (A-7)
 *	== TASK4 ==
 *	11:	wup_tsk(TASK11)											... (A-9)
 *	//		高：なし，中：TASK11，TASK2，TASK4→TASK3
 *	== TASK11 ==
 *	12:	sus_tsk(TASK3)
 *		sus_tsk(TASK4)
 *	//		高：なし，中：TASK11，TASK2
 *	13:	sta_alm(ALM1, TEST_TIME_PROC) ... ALM1-1が実行開始するまで
 *		slp_tsk()												... (A-4)
 *	//		高：なし，中：TASK2
 *	// 実行できるタスクなし
 *	== ALM1-1 ==
 *	14:	wup_tsk(TASK11)
 *		RETURN													... (A-13)
 *	== TASK11 ==
 *	15:	wup_tsk(TASK51)
 *	//		高：なし，中：TASK51，TASK11，TASK2
 *	== TASK51 ==
 *	16:	sus_tsk(TASK11)
 *		sta_alm(ALM1, TEST_TIME_PROC) ... ALM1-2が実行開始するまで
 *		slp_tsk()												... (A-8)
 *	//		高：なし，中：TASK2
 *	// 実行できるタスクなし
 *	== ALM1-2 ==
 *	17:	wup_tsk(TASK51)
 *		RETURN													... (A-14)
 *	== TASK51 ==
 *	18:	rsm_tsk(TASK11)
 *		slp_tsk()
 *	== TASK11 ==
 *	19:	rsm_tsk(TASK3)
 *		slp_tsk()
 *	== TASK3 ==
 *	20:	sta_alm(ALM1, TEST_TIME_PROC) ... ALM1-3が実行開始するまで
 *		slp_tsk()												... (A-12)
 *	//		高：なし，中：TASK2
 *	// 実行できるタスクなし
 *	== ALM1-3 ==
 *	21:	wup_tsk(TASK3)
 *		RETURN													... (A-15)
 *	== TASK3 ==
 *	22:	DO(WAIT(task3_flag))
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	23:	DO(SET(task3_flag))
 *		wup_tsk(TASK11)
 *		DO(WAIT(task2_flag))
 *	//		高：なし，中：TASK11，TASK2，TASK3
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	24:	rsm_tsk(TASK4)
 *		chg_pri(TASK4, HIGH_PRIORITY)							... (B-5)
 *	//		高：TASK4，中：TASK11，TASK2，TASK3
 *	== TASK4 ==
 *	25:	wup_tsk(TASK51)
 *	//		高：TASK4，中：TASK51，TASK11，TASK2，TASK3
 *	26:	wup_tsk(TASK52)											... (B-4)
 *	//		高：TASK52，TASK4，中：TASK51，TASK11，TASK2，TASK3
 *	== TASK52 ==
 *	27:	sus_tsk(TASK4)
 *		slp_tsk()												... (B-1)
 *	//		高：なし，中：TASK51，TASK11，TASK2，TASK3
 *	== TASK51 ==
 *	28:	rsm_tsk(TASK4)											... (B-2)
 *	//		高：TASK4，中：TASK51，TASK11，TASK2，TASK3
 *	== TASK4 ==
 *	29:	slp_tsk()												... (B-4)
 *	== TASK51 ==
 *	30:	slp_tsk()												... (B-2)
 *	//		高：なし，中：TASK11，TASK2，TASK3
 *	== TASK3 ==
 *	31:	sta_alm(ALM1, TEST_TIME_PROC) ... ALM1-4が実行開始するまで
 *		slp_tsk()												... (B-6)
 *	//		高：なし，中：TASK11，TASK2
 *	// 実行できるタスクなし
 *	== ALM1-4 ==
 *	32:	wup_tsk(TASK3)
 *		RETURN													... (B-8)
 *	== TASK3 ==
 *	33:	wup_tsk(TASK51)
 *	//		高：なし，中：TASK51，TASK11，TASK2，TASK3
 *	== TASK51 ==
 *	34:	sus_tsk(TASK3)
 *		sta_alm(ALM1, TEST_TIME_PROC) ... ALM1-5が実行開始するまで
 *		slp_tsk()												... (B-3)
 *	//		高：なし，中：TASK11，TASK2
 *	// 実行できるタスクなし
 *	== ALM1-5 ==
 *	35:	wup_tsk(TASK51)
 *		RETURN													... (B-7)
 *	== TASK51 ==
 *	36:	rsm_tsk(TASK3)
 *		slp_tsk()
 *	== TASK3 ==
 *	37:	slp_tsk()
 *	// タイムウィンドウ for DOM1
 *	== TASK11 ==
 *	38:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_tprot2.h"

volatile bool_t	task11_flag;
volatile bool_t	task12_flag;
volatile bool_t	task2_flag;
volatile bool_t	task3_flag;

#define WAIT(flag)	do { (flag) = false; while (!(flag)); } while (false)
#define SET(flag)	do { (flag) = true; } while (false)

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

static uint_t	alarm1_count = 0;

void
alarm1_handler(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++alarm1_count) {
	case 1:
		check_point(14);
		ercd = wup_tsk(TASK11);
		check_ercd(ercd, E_OK);

		return;

		check_point(0);

	case 2:
		check_point(17);
		ercd = wup_tsk(TASK51);
		check_ercd(ercd, E_OK);

		return;

		check_point(0);

	case 3:
		check_point(21);
		ercd = wup_tsk(TASK3);
		check_ercd(ercd, E_OK);

		return;

		check_point(0);

	case 4:
		check_point(32);
		ercd = wup_tsk(TASK3);
		check_ercd(ercd, E_OK);

		return;

		check_point(0);

	case 5:
		check_point(35);
		ercd = wup_tsk(TASK51);
		check_ercd(ercd, E_OK);

		return;

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}

void
task11(intptr_t exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(7);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(12);
	ercd = sus_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = sus_tsk(TASK4);
	check_ercd(ercd, E_OK);

	check_point(13);
	ercd = sta_alm(ALM1, TEST_TIME_PROC);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(15);
	ercd = wup_tsk(TASK51);
	check_ercd(ercd, E_OK);

	check_point(19);
	ercd = rsm_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_finish(38);
	check_point(0);
}

void
task12(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(2);
	ercd = act_tsk(TASK51);
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK52);
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(23);
	SET(task3_flag);

	ercd = wup_tsk(TASK11);
	check_ercd(ercd, E_OK);

	WAIT(task2_flag);

	check_point(0);
}

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(8);
	ercd = act_tsk(TASK4);
	check_ercd(ercd, E_OK);

	ercd = rot_rdq(TPRI_SELF);
	check_ercd(ercd, E_OK);

	check_point(20);
	ercd = sta_alm(ALM1, TEST_TIME_PROC);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(22);
	WAIT(task3_flag);

	check_point(24);
	ercd = rsm_tsk(TASK4);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TASK4, HIGH_PRIORITY);
	check_ercd(ercd, E_OK);

	check_point(31);
	ercd = sta_alm(ALM1, TEST_TIME_PROC);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(33);
	ercd = wup_tsk(TASK51);
	check_ercd(ercd, E_OK);

	check_point(37);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task4(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(9);
	ercd = wup_tsk(TASK51);
	check_ercd(ercd, E_OK);

	check_point(11);
	ercd = wup_tsk(TASK11);
	check_ercd(ercd, E_OK);

	check_point(25);
	ercd = wup_tsk(TASK51);
	check_ercd(ercd, E_OK);

	check_point(26);
	ercd = wup_tsk(TASK52);
	check_ercd(ercd, E_OK);

	check_point(29);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task51(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(4);
	ercd = rsm_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(6);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(10);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(16);
	ercd = sus_tsk(TASK11);
	check_ercd(ercd, E_OK);

	ercd = sta_alm(ALM1, TEST_TIME_PROC);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(18);
	ercd = rsm_tsk(TASK11);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(28);
	ercd = rsm_tsk(TASK4);
	check_ercd(ercd, E_OK);

	check_point(30);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(34);
	ercd = sus_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = sta_alm(ALM1, TEST_TIME_PROC);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(36);
	ercd = rsm_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task52(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(3);
	ercd = sus_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(27);
	ercd = sus_tsk(TASK4);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}
