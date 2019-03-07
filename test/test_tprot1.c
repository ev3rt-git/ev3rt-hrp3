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
 *  $Id: test_tprot1.c 614 2018-12-20 12:30:12Z ertl-hiro $
 */

/* 
 *		時間パーティショニングに関するテスト(1)
 *
 * 【テストの目的】
 *
 *  時間パーティショニングを使用した時のタイムウィンドウ／アイドルウィ
 *  ンドウ切換えに関して，システム動作モードを変更しない場合に限定して，
 *  様々な状況を網羅的にテストする．
 *
 * 【テスト項目】
 *
 *	(A) タイムウィンドウ1からタイムウィンドウ2に切り換わる時
 *		(A-1) ユーザドメインのタスク→ユーザドメインのタスク
 *		(A-2) ユーザドメインのタスク→アイドルドメインのタスク
 *		(A-3) ユーザドメインのタスク→実行できるタスクなし
 *		(A-4) アイドルドメインのタスク→ユーザドメインのタスク
 *		(A-5) アイドルドメインのタスク→アイドルドメインのタスク
 *		(A-6) 実行できるタスクなし→ユーザドメインのタスク
 *		(A-7) 実行できるタスクなし→実行できるタスクなし
 *	(B) タイムウィンドウからアイドルウィンドウに切り換わる時
 *		(B-1) ユーザドメインのタスク→アイドルドメインのタスク
 *		(B-2) ユーザドメインのタスク→実行できるタスクなし
 *		(B-3) アイドルドメインのタスク→アイドルドメインのタスク
 *		(B-4) 実行できるタスクなし→実行できるタスクなし
 *	(C) アイドルウィンドウからタイムウィンドウに切り換わる時
 *		(C-1) アイドルドメインのタスク→ユーザドメインのタスク
 *		(C-2) アイドルドメインのタスク→アイドルドメインのタスク
 *		(C-3) 実行できるタスクなし→ユーザドメインのタスク
 *		(C-4) 実行できるタスクなし→実行できるタスクなし
 *
 * 【使用リソース】
 *
 *	システム周期: (20 * TEST_TIME_CP)
 *	DOM1: システム周期の最初タイムウィンドウ（長さ: 5 * TEST_TIME_CP）
 *		  を割当て
 *	DOM2: システム周期の2番目のタイムウィンドウ（長さ: 5 * TEST_TIME_CP）
 *		  を割当て
 *	DOM3: タイムウィンドウを割り当てない
 *	TASK1: DOM1，中優先度，TA_ACT属性
 *	TASK2: DOM2，中優先度，TA_ACT属性
 *	TASK3: DOM3，中優先度，TA_ACT属性
 *	CYC1: 監視用，カーネルドメイン，初期起動: 4 * TEST_TIME_CP，
 *		  周期: 5 * TEST_TIME_CP
 *
 * 【テストシーケンス】
 *
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	1:	get_tid(&tskid)
 *		assert(tskid == TASK1)
 *		get_did(&domid)
 *		assert(domid == DOM1)
 *		DO(WAIT(task1_flag))
 *	== CYC1-1 ==
 *	2:	get_tid(&tskid)
 *		assert(tskid == TASK1)
 *		get_did(&domid)
 *		assert(domid == DOM1)
 *		RETURN
 *	// タイムウィンドウ for DOM2									... (A-1)
 *	== TASK2 ==
 *	3:	DO(WAIT(task2_flag))
 *	== CYC1-2 ==
 *	4:	get_tid(&tskid)
 *		assert(tskid == TASK2)
 *		get_did(&domid)
 *		assert(domid == DOM2)
 *		RETURN
 *	// アイドルウィンドウ											... (B-1)
 *	== TASK3 ==
 *	5:	DO(SET(task1_flag))
 *		DO(SET(task2_flag))
 *		DO(WAIT(task3_flag))
 *	== CYC1-3 ==
 *		RETURN
 *	== CYC1-4 ==
 *	6:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// タイムウィンドウ for DOM1									... (C-1)
 *	== TASK1 ==
 *	7:	DO(SET(task3_flag))
 *		sus_tsk(TASK2)
 *		DO(WAIT(task1_flag))
 *	== CYC1-5 ==
 *	8:	get_tid(&tskid)
 *		assert(tskid == TASK1)
 *		get_did(&domid)
 *		assert(domid == DOM1)
 *		RETURN
 *	// タイムウィンドウ for DOM2									... (A-2)
 *	== TASK3 ==
 *	9:	DO(SET(task1_flag))
 *		DO(WAIT(task3_flag))
 *	== CYC1-6 ==
 *	10:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// アイドルウィンドウ											... (B-3)
 *	== CYC1-7 ==
 *		RETURN
 *	== CYC1-8 ==
 *	11:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// タイムウィンドウ for DOM1									... (C-1)
 *	== TASK1 ==
 *	12:	DO(SET(task3_flag))
 *		sus_tsk(TASK3)
 *		DO(WAIT(task1_flag))
 *	== CYC1-9 ==
 *	13:	get_tid(&tskid)
 *		assert(tskid == TASK1)
 *		get_did(&domid)
 *		assert(domid == DOM1)
 *		RETURN
 *	// タイムウィンドウ for DOM2									... (A-3)
 *	== CYC1-10 ==
 *	14:	get_tid(&tskid)
 *		assert(tskid == TSK_NONE)
 *		get_did(&domid)
 *		assert(domid == TDOM_NONE)
 *		RETURN
 *	// アイドルウィンドウ											... (B-4)
 *	== CYC1-11 ==
 *		RETURN
 *	== CYC1-12 ==
 *	15:	get_tid(&tskid)
 *		assert(tskid == TSK_NONE)
 *		get_did(&domid)
 *		assert(domid == TDOM_NONE)
 *		DO(SET(task1_flag))
 *		RETURN
 *	// タイムウィンドウ for DOM1									... (C-3)
 *	== TASK1 ==
 *	16:	rsm_tsk(TASK2)
 *		rsm_tsk(TASK3)
 *		slp_tsk()
 *	== TASK3 ==
 *	17:	DO(WAIT(task3_flag))
 *	== CYC1-13 ==
 *	18:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// タイムウィンドウ for DOM2									... (A-4)
 *	== TASK2 ==
 *	19:	DO(SET(task3_flag))
 *		sus_tsk(TASK3)
 *		DO(WAIT(task2_flag))
 *	== CYC1-14 ==
 *	20:	get_tid(&tskid)
 *		assert(tskid == TASK2)
 *		get_did(&domid)
 *		assert(domid == DOM2)
 *		RETURN
 *	// アイドルウィンドウ											... (B-2)
 *	== CYC1-15 ==
 *		RETURN
 *	== CYC1-16 ==
 *	21:	get_tid(&tskid)
 *		assert(tskid == TSK_NONE)
 *		get_did(&domid)
 *		assert(domid == TDOM_NONE)
 *		RETURN
 *	// タイムウィンドウ for DOM1									... (C-4)
 *	== CYC1-17 ==
 *	22:	get_tid(&tskid)
 *		assert(tskid == TSK_NONE)
 *		get_did(&domid)
 *		assert(domid == TDOM_NONE)
 *		DO(SET(task2_flag))
 *		RETURN
 *	// タイムウィンドウ for DOM2									... (A-6)
 *	== TASK2 ==
 *	23:	rsm_tsk(TASK3)
 *		slp_tsk()
 *	== TASK3 ==
 *	24:	DO(WAIT(task3_flag))
 *	== CYC1-18 ==
 *	25:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// アイドルウィンドウ											... (B-3)
 *	== CYC1-19 ==
 *		RETURN
 *	== CYC1-20 ==
 *	26:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// タイムウィンドウ for DOM1									... (C-2)
 *	== CYC1-21 ==
 *	27:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// タイムウィンドウ for DOM2									... (A-5)
 *	== CYC1-22 ==
 *	28:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		RETURN
 *	// アイドルウィンドウ											... (B-3)
 *	== CYC1-23 ==
 *		RETURN
 *	== CYC1-24 ==
 *	29:	get_tid(&tskid)
 *		assert(tskid == TASK3)
 *		get_did(&domid)
 *		assert(domid == DOM3)
 *		wup_tsk(TASK1)
 *		RETURN
 *	// タイムウィンドウ for DOM1									... (C-1)
 *	== TASK1 ==
 *	30:	DO(SET(task3_flag))
 *		sus_tsk(TASK3)
 *		slp_tsk()
 *	== CYC1-25 ==
 *	31:	get_tid(&tskid)
 *		assert(tskid == TSK_NONE)
 *		get_did(&domid)
 *		assert(domid == TDOM_NONE)
 *		RETURN
 *	// タイムウィンドウ for DOM2									... (A-7)
 *	== CYC1-26 ==
 *	32:	get_tid(&tskid)
 *		assert(tskid == TSK_NONE)
 *		get_did(&domid)
 *		assert(domid == TDOM_NONE)
 *	33:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_tprot1.h"

volatile bool_t	task1_flag;
volatile bool_t	task2_flag;
volatile bool_t	task3_flag;

#define WAIT(flag)	do { (flag) = false; while (!(flag)); } while (false)
#define SET(flag)	do { (flag) = true; } while (false)

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

static uint_t	cyclic1_count = 0;

void
cyclic1_handler(intptr_t exinf)
{
	ER_UINT	ercd;
	ID		tskid;
	ID		domid;

	switch (++cyclic1_count) {
	case 1:
		check_point(2);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK1);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM1);

		return;

		check_assert(false);

	case 2:
		check_point(4);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK2);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM2);

		return;

		check_assert(false);

	case 3:
		return;

		check_assert(false);

	case 4:
		check_point(6);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 5:
		check_point(8);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK1);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM1);

		return;

		check_assert(false);

	case 6:
		check_point(10);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 7:
		return;

		check_assert(false);

	case 8:
		check_point(11);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 9:
		check_point(13);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK1);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM1);

		return;

		check_assert(false);

	case 10:
		check_point(14);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TSK_NONE);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == TDOM_NONE);

		return;

		check_assert(false);

	case 11:
		return;

		check_assert(false);

	case 12:
		check_point(15);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TSK_NONE);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == TDOM_NONE);

		SET(task1_flag);

		return;

		check_assert(false);

	case 13:
		check_point(18);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 14:
		check_point(20);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK2);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM2);

		return;

		check_assert(false);

	case 15:
		return;

		check_assert(false);

	case 16:
		check_point(21);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TSK_NONE);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == TDOM_NONE);

		return;

		check_assert(false);

	case 17:
		check_point(22);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TSK_NONE);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == TDOM_NONE);

		SET(task2_flag);

		return;

		check_assert(false);

	case 18:
		check_point(25);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 19:
		return;

		check_assert(false);

	case 20:
		check_point(26);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 21:
		check_point(27);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 22:
		check_point(28);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		return;

		check_assert(false);

	case 23:
		return;

		check_assert(false);

	case 24:
		check_point(29);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TASK3);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == DOM3);

		ercd = wup_tsk(TASK1);
		check_ercd(ercd, E_OK);

		return;

		check_assert(false);

	case 25:
		check_point(31);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TSK_NONE);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == TDOM_NONE);

		return;

		check_assert(false);

	case 26:
		check_point(32);
		ercd = get_tid(&tskid);
		check_ercd(ercd, E_OK);

		check_assert(tskid == TSK_NONE);

		ercd = get_did(&domid);
		check_ercd(ercd, E_OK);

		check_assert(domid == TDOM_NONE);

		check_finish(33);
		check_assert(false);

	default:
		check_assert(false);
	}
	check_assert(false);
}

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;
	ID		tskid;
	ID		domid;

	test_start(__FILE__);

	check_point(1);
	ercd = get_tid(&tskid);
	check_ercd(ercd, E_OK);

	check_assert(tskid == TASK1);

	ercd = get_did(&domid);
	check_ercd(ercd, E_OK);

	check_assert(domid == DOM1);

	WAIT(task1_flag);

	check_point(7);
	SET(task3_flag);

	ercd = sus_tsk(TASK2);
	check_ercd(ercd, E_OK);

	WAIT(task1_flag);

	check_point(12);
	SET(task3_flag);

	ercd = sus_tsk(TASK3);
	check_ercd(ercd, E_OK);

	WAIT(task1_flag);

	check_point(16);
	ercd = rsm_tsk(TASK2);
	check_ercd(ercd, E_OK);

	ercd = rsm_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(30);
	SET(task3_flag);

	ercd = sus_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_assert(false);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(3);
	WAIT(task2_flag);

	check_point(19);
	SET(task3_flag);

	ercd = sus_tsk(TASK3);
	check_ercd(ercd, E_OK);

	WAIT(task2_flag);

	check_point(23);
	ercd = rsm_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_assert(false);
}

void
task3(intptr_t exinf)
{

	check_point(5);
	SET(task1_flag);

	SET(task2_flag);

	WAIT(task3_flag);

	check_point(9);
	SET(task1_flag);

	WAIT(task3_flag);

	check_point(17);
	WAIT(task3_flag);

	check_point(24);
	WAIT(task3_flag);

	check_assert(false);
}
