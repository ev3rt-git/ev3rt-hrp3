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
 *  $Id: test_tprot4.c 505 2018-10-27 14:01:12Z ertl-hiro $
 */

/* 
 *		時間パーティショニングに関するテスト(4)
 *
 * 【テストの目的】
 *
 *  時間パーティショニングを使用した時のタイムイベント通知に関して，シ
 *  ステム動作モードを変更しない場合に限定して，タイムイベント通知が期
 *  待したタイミングで実行されること，タイムイベント通知がタイムウィン
 *  ドウを使い切った場合の処理についてテストする．
 *
 * 【テスト項目】
 *
 *	(A) タイムイベント通知が期待したタイミングで実行されること
 *		(A-1) ユーザドメイン（アイドルドメイン以外）のタイムイベント
 *		(A-2) アイドルドメインのタイムイベント
 *		(A-3) カーネルドメインのタイムイベント
 *	(B) タイムイベント通知がタイムウィンドウを使い切った場合の処理
 *		(B-1) ユーザドメイン（アイドルドメイン以外）のタイムイベント
 *		(B-2) アイドルドメインのタイムイベント
 *
 * 【使用リソース】
 *
 *	システム周期: (20 * TEST_TIME_CP)
 *	DOM1: システム周期の最初タイムウィンドウ（長さ: 5 * TEST_TIME_CP）
 *		  を割当て
 *	DOM2: システム周期の2番目のタイムウィンドウ（長さ: 5 * TEST_TIME_CP）
 *		  を割当て
 *	DOM3: タイムウィンドウを割り当てない
 *	DOM4: タイムウィンドウを割り当てない
 *	TASK11: DOM1，中優先度，TA_ACT属性
 *	TASK12: DOM1，高優先度
 *	TASK2: DOM2，中優先度，TA_ACT属性
 *	TASK3: DOM3，中優先度，TA_ACT属性
 *	CYC11: DOM1，TASK12を起動，TA_STA属性，初期起動：0，
 *		   周期：(20 * TEST_TIME_CP)
 *	CYC12: DOM1，SEM1にsignal，初期起動：0，周期：1（過負荷な周期ハンドラ）
 *	CYC3: DOM3，SEM3にsignal，初期起動：0，周期：1（過負荷な周期ハンドラ）
 *	ALM1: DOM1，FLG1の0x01をセット
 *	ALM2: DOM2，FLG1の0x02をセット
 *	ALM3: DOM3，FLG1の0x04をセット
 *	ALM4: DOM4，FLG1の0x08をセット
 *	ALM5: カーネルドメイン，FLG1の0x10をセット
 *	FLG1: テスト用イベントフラグ
 *	SIG1: テスト用セマフォ
 *
 * 【テストシーケンス】
 *
 *	// タイムウィンドウ for DOM1
 *	// CYC11が実行される
 *	== TASK12-1 ==
 *	1:	ext_tsk()
 *	== TASK11 ==
 *	2:	DO(WAIT(task11_flag))
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	3:	DO(WAIT(task2_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	4:	DO(SET(task11_flag))
 *		DO(SET(task2_flag))
 *		DO(WAIT(task3_flag))
 *	// タイムウィンドウ for DOM1
 *	// CYC11が実行される
 *	== TASK12-2 ==
 *	5:	ext_tsk()
 *	== TASK11 ==
 *	6:	DO(SET(task3_flag))
 *		set_flg(FLG1, 0x80U)
 *		sta_alm(ALM1, 500U)
 *		sta_alm(ALM2, 500U)
 *		sta_alm(ALM3, 500U)
 *		sta_alm(ALM4, 500U)
 *		sta_alm(ALM5, 500U)
 *		pol_flg(FLG1, 0xffU, TWF_ORW, &flgptn)
 *		assert(flgptn == 0x80U)
 *		DO(flgptn = poll_flag_change(FLG1, flgptn))
 *	// ALM5が実行される											... (A-3)
 *	7:	assert(flgptn == 0x90U)
 *		DO(flgptn = poll_flag_change(FLG1, flgptn))
 *	// タイムウィンドウ for DOM2
 *	// ALM2が実行される											... (A-1)
 *	== TASK2 ==
 *	8:	pol_flg(FLG1, 0xffU, TWF_ORW, &flgptn)
 *		assert(flgptn == 0x92U)
 *		DO(flgptn = poll_flag_change(FLG1, flgptn))
 *	// アイドルウィンドウ
 *	// ALM3とALM4が実行される									... (A-2)
 *	== TASK3 ==
 *	9:	pol_flg(FLG1, 0xffU, TWF_ORW, &flgptn)
 *		assert(flgptn == 0x9eU)
 *		DO(flgptn = poll_flag_change(FLG1, flgptn))
 *	// タイムウィンドウ for DOM1
 *	// CYC11とALM1が実行される									... (A-1)
 *	== TASK12-3 ==
 *	10:	ext_tsk()
 *	== TASK11 ==
 *	11:	assert(flgptn == 0x9fU)
 *		DO(WAIT(task11_flag))
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	12:	assert(flgptn == 0x9fU)
 *		DO(WAIT(task2_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	13:	assert(flgptn == 0x9fU)
 *		DO(SET(task11_flag))
 *		DO(SET(task2_flag))
 *		sta_cyc(CYC12)
 *		DO(WAIT(task3_flag))
 *	// タイムウィンドウ for DOM1
 *	// CYC12が実行される．CYC12がタイムウィンドウを使い切る	... (B-1)
 *	// CYC11は実行されない．
 *	// TASK11が一瞬実行される可能性がある
 *	== TASK11 ==
 *		DO(WAIT_WO_RESET(task11_flag_1))
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	14:	DO(SET(task3_flag_1))
 *		DO(SET(task11_flag_1))
 *		stp_cyc(CYC12)
 *		sta_cyc(CYC3)
 *		ref_sem(SEM1, &pk_rsem)
 *		DO(syslog_1(LOG_NOTICE, "semaphore count of SEM1 = %d", pk_rsem.semcnt))
 *		DO(WAIT(task2_flag))
 *	// アイドルウィンドウ
 *	// CYC3が実行される．CYC3がタイムウィンドウを使い切る		... (B-2)
 *	// TASK3が一瞬実行される可能性がある
 *	== TASK3 ==
 *		DO(WAIT_WO_RESET(task3_flag_1))
 *	// タイムウィンドウ for DOM1
 *	// CYC11が2回実行される．
 *	== TASK12-4 ==
 *	15:	can_act(TSK_SELF) -> 1
 *		ext_tsk()
 *	== TASK11 ==
 *	16:	stp_cyc(CYC3)
 *		ref_sem(SEM3, &pk_rsem)
 *		DO(syslog_1(LOG_NOTICE, "semaphore count of SEM3 = %d", pk_rsem.semcnt))
 *	17:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_tprot4.h"

volatile bool_t	task11_flag;
volatile bool_t	task12_flag;
volatile bool_t	task2_flag;
volatile bool_t	task3_flag;

#define WAIT(flag)	do { (flag) = false; while (!(flag)); } while (false)
#define SET(flag)	do { (flag) = true; } while (false)

volatile bool_t	task11_flag_1 = false;
volatile bool_t	task3_flag_1 = false;

#define WAIT_WO_RESET(flag)		do { while (!(flag)); } while (false)

FLGPTN
poll_flag_change(ID flgid, FLGPTN orig_flgptn)
{
	ER		ercd;
	FLGPTN	flgptn;

	do {
		ercd = pol_flg(flgid, 0xffU, TWF_ORW, &flgptn);
		assert(ercd == E_OK);
	} while (flgptn == orig_flgptn);
	return(flgptn);
}

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task11(intptr_t exinf)
{
	ER_UINT	ercd;
	FLGPTN	flgptn;
	T_RSEM	pk_rsem;

	check_point(2);
	WAIT(task11_flag);

	check_point(6);
	SET(task3_flag);

	ercd = set_flg(FLG1, 0x80U);
	check_ercd(ercd, E_OK);

	ercd = sta_alm(ALM1, 500U);
	check_ercd(ercd, E_OK);

	ercd = sta_alm(ALM2, 500U);
	check_ercd(ercd, E_OK);

	ercd = sta_alm(ALM3, 500U);
	check_ercd(ercd, E_OK);

	ercd = sta_alm(ALM4, 500U);
	check_ercd(ercd, E_OK);

	ercd = sta_alm(ALM5, 500U);
	check_ercd(ercd, E_OK);

	ercd = pol_flg(FLG1, 0xffU, TWF_ORW, &flgptn);
	check_ercd(ercd, E_OK);

	check_assert(flgptn == 0x80U);

	flgptn = poll_flag_change(FLG1, flgptn);

	check_point(7);
	check_assert(flgptn == 0x90U);

	flgptn = poll_flag_change(FLG1, flgptn);

	check_point(11);
	check_assert(flgptn == 0x9fU);

	WAIT(task11_flag);

	WAIT_WO_RESET(task11_flag_1);

	check_point(16);
	ercd = stp_cyc(CYC3);
	check_ercd(ercd, E_OK);

	ercd = ref_sem(SEM3, &pk_rsem);
	check_ercd(ercd, E_OK);

	syslog_1(LOG_NOTICE, "semaphore count of SEM3 = %d", pk_rsem.semcnt);

	check_finish(17);
	check_point(0);
}

static uint_t	task12_count = 0;

void
task12(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++task12_count) {
	case 1:
		test_start(__FILE__);

		check_point(1);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 2:
		check_point(5);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 3:
		check_point(10);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 4:
		check_point(15);
		ercd = can_act(TSK_SELF);
		check_ercd(ercd, 1);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;
	FLGPTN	flgptn;
	T_RSEM	pk_rsem;

	check_point(3);
	WAIT(task2_flag);

	check_point(8);
	ercd = pol_flg(FLG1, 0xffU, TWF_ORW, &flgptn);
	check_ercd(ercd, E_OK);

	check_assert(flgptn == 0x92U);

	flgptn = poll_flag_change(FLG1, flgptn);

	check_point(12);
	check_assert(flgptn == 0x9fU);

	WAIT(task2_flag);

	check_point(14);
	SET(task3_flag_1);

	SET(task11_flag_1);

	ercd = stp_cyc(CYC12);
	check_ercd(ercd, E_OK);

	ercd = sta_cyc(CYC3);
	check_ercd(ercd, E_OK);

	ercd = ref_sem(SEM1, &pk_rsem);
	check_ercd(ercd, E_OK);

	syslog_1(LOG_NOTICE, "semaphore count of SEM1 = %d", pk_rsem.semcnt);

	WAIT(task2_flag);

	check_point(0);
}

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;
	FLGPTN	flgptn;

	check_point(4);
	SET(task11_flag);

	SET(task2_flag);

	WAIT(task3_flag);

	check_point(9);
	ercd = pol_flg(FLG1, 0xffU, TWF_ORW, &flgptn);
	check_ercd(ercd, E_OK);

	check_assert(flgptn == 0x9eU);

	flgptn = poll_flag_change(FLG1, flgptn);

	check_point(13);
	check_assert(flgptn == 0x9fU);

	SET(task11_flag);

	SET(task2_flag);

	ercd = sta_cyc(CYC12);
	check_ercd(ercd, E_OK);

	WAIT(task3_flag);

	WAIT_WO_RESET(task3_flag_1);

	check_point(0);
}
