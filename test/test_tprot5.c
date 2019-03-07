/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2018,2019 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_tprot5.c 630 2019-01-02 08:54:52Z ertl-hiro $
 */

/* 
 *		時間パーティショニングに関するテスト(5)
 *
 * 【テストの目的】
 *
 *  タスクディスパッチの保留により，システム周期切換え処理とタイムウィ
 *  ンドウ切換え処理がペンディングされた場合の振舞いをテストする．
 *
 * 【テスト項目】
 *
 *	(A) システム周期切換え処理がペンディングされた場合
 *		(A-1) ena_dspによるタスクディスパッチの保留解除
 *		(A-2) chg_ipmによるタスクディスパッチの保留解除
 *		(A-3) ext_tskによるタスクディスパッチの保留解除
 *	(B) タイムウィンドウ切換え処理がペンディングされた場合
 *		(B-1) ユーザドメイン→ユーザドメインの切換え
 *			(B-1-1) ena_dspによるタスクディスパッチの保留解除
 *			(B-1-2) chg_ipmによるタスクディスパッチの保留解除
 *			(B-1-3) ext_tskによるタスクディスパッチの保留解除
 *		(B-2) ユーザドメイン→アイドルドメインの切換え
 *			(B-2-1) ena_dspによるタスクディスパッチの保留解除
 *			(B-2-2) chg_ipmによるタスクディスパッチの保留解除
 *			(B-2-3) ext_tskによるタスクディスパッチの保留解除
 *	(C) chg_somによるシステム周期停止モードからのシステム周期の実行開
 *		始処理がペンディングされた場合
 *		(C-1) ena_dspによるタスクディスパッチの保留解除
 *		(C-2) chg_ipmによるタスクディスパッチの保留解除
 *		(C-3) ext_tskによるタスクディスパッチの保留解除
 *
 * 【使用リソース】
 *
 *	システム周期: (20 * TEST_TIME_CP)
 *	DOM1: システム周期の最初タイムウィンドウ（長さ: 5 * TEST_TIME_CP）
 *		  を割当て
 *	DOM2: システム周期の2番目のタイムウィンドウ（長さ: 5 * TEST_TIME_CP）
 *		  を割当て
 *	DOM3: タイムウィンドウを割り当てない
 *	SOM1: 初期モード，遷移先システム動作モード: SOM1，DOM2→DOM1→アイドル
 *	TASK1: DOM1，中優先度，TA_ACT属性
 *	TASK2: DOM2，中優先度，TA_ACT属性
 *	TASK3: DOM3，中優先度，TA_ACT属性
 *	TASK4: カーネルドメイン，低優先度
 *
 * 【テストシーケンス】
 *
 *	// システム動作モード：SOM1
 *	// タイムウィンドウ for DOM1
 *	== TASK1-1 ==
 *	1:	dis_dsp()
 *		DO(while(fetch_pending_twdswitch() == false);)
 *		// タイムウィンドウタイマ割込み → ペンディング
 *	2:	ena_dsp()													... (B-1-1)
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	3:	dis_dsp()
 *		DO(while(fetch_pending_twdswitch() == false);)
 *		// タイムウィンドウタイマ割込み → ペンディング
 *	4:	ena_dsp()													... (B-2-1)
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	5:	dis_dsp()
 *		DO(while(fetch_pending_scycswitch() == false);)
 *		// システム周期切換え割込み → ペンディング
 *	6:	ena_dsp()													... (A-1)
 *	// タイムウィンドウ for DOM1
 *	== TASK1-1 ==
 *	7:	chg_ipm(TMAX_INTPRI)
 *		DO(while(fetch_pending_twdswitch() == false);)
 *		// タイムウィンドウタイマ割込み → ペンディング
 *	8:	chg_ipm(TIPM_ENAALL)										... (B-1-2)
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	9:	chg_ipm(TMAX_INTPRI)
 *		DO(while(fetch_pending_twdswitch() == false);)
 *		// タイムウィンドウタイマ割込み → ペンディング
 *	10:	chg_ipm(TIPM_ENAALL)										... (B-2-2)
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	11:	chg_ipm(TMAX_INTPRI)
 *		DO(while(fetch_pending_scycswitch() == false);)
 *		// システム周期切換え割込み → ペンディング
 *	12:	chg_ipm(TIPM_ENAALL)										... (A-2)
 *	// タイムウィンドウ for DOM1
 *	== TASK1-1 ==
 *	13:	act_tsk(TASK1)
 *		dis_dsp()
 *		DO(while(fetch_pending_twdswitch() == false);)
 *		// タイムウィンドウタイマ割込み → ペンディング
 *	14:	ext_tsk()													... (B-1-3)
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	15:	dis_dsp()
 *		DO(while(fetch_pending_twdswitch() == false);)
 *		// タイムウィンドウタイマ割込み → ペンディング
 *	16:	ext_tsk()													... (B-2-3)
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	17:	dis_dsp()
 *		DO(while(fetch_pending_scycswitch() == false);)
 *		// システム周期切換え割込み → ペンディング
 *	18:	ext_tsk()													... (A-3)
 *	// タイムウィンドウ for DOM1
 *	== TASK1-2 ==
 *	19:	act_tsk(TASK4)
 *	20:	ext_tsk()
 *	== TASK4 ==
 *	21:	chg_som(TSOM_STP)
 *		dly_tsk(21 * TEST_TIME_CP)
 *	// タイムウィンドウ for DOM2
 *	// アイドルウィンドウ
 *	// システム周期停止モード
 *	== TASK4 ==
 *	22:	dis_dsp()
 *		chg_som(SOM1)
 *		assert(fetch_pending_scycswitch() == true)
 *		act_tsk(TASK1)
 *	23:	ena_dsp()													... (C-1)
 *	// タイムウィンドウ for DOM1
 *	== TASK1-3 ==
 *	24:	ext_tsk()
 *	== TASK4 ==
 *	25:	chg_som(TSOM_STP)
 *		dly_tsk(21 * TEST_TIME_CP)
 *	// タイムウィンドウ for DOM2
 *	// アイドルウィンドウ
 *	// システム周期停止モード
 *	== TASK4 ==
 *	26:	chg_ipm(TMAX_INTPRI)
 *		chg_som(SOM1)
 *		assert(fetch_pending_scycswitch() == true)
 *		act_tsk(TASK1)
 *	27:	chg_ipm(TIPM_ENAALL)										... (C-2)
 *	// タイムウィンドウ for DOM1
 *	== TASK1-4 ==
 *	28:	ext_tsk()
 *	== TASK4 ==
 *	29:	chg_som(TSOM_STP)
 *		dly_tsk(21 * TEST_TIME_CP)
 *	// タイムウィンドウ for DOM2
 *	// アイドルウィンドウ
 *	// システム周期停止モード
 *	== TASK4 ==
 *	30:	dis_dsp()
 *		chg_som(SOM1)
 *		assert(fetch_pending_scycswitch() == true)
 *		act_tsk(TASK1)
 *	31:	ext_tsk()													... (C-3)
 *	// タイムウィンドウ for DOM1
 *	== TASK1-5 ==
 *	32:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_common.h"

extern bool_t	_kernel_pending_scycswitch;
extern bool_t	_kernel_pending_twdswitch;

ER_UINT extsvc1_routine(intptr_t par1, intptr_t par2, intptr_t par3,
									intptr_t par4, intptr_t par5, ID cdmid)
{
	return(_kernel_pending_scycswitch);
}

ER_UINT extsvc2_routine(intptr_t par1, intptr_t par2, intptr_t par3,
									intptr_t par4, intptr_t par5, ID cdmid)
{
	return(_kernel_pending_twdswitch);
}

static bool_t
fetch_pending_scycswitch(void)
{
	return((bool_t) cal_svc(TFN_EXTSVC1, 0, 0, 0, 0, 0));
}

static bool_t
fetch_pending_twdswitch(void)
{
	return((bool_t) cal_svc(TFN_EXTSVC2, 0, 0, 0, 0, 0));
}

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

static uint_t	task1_count = 0;

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++task1_count) {
	case 1:
		test_start(__FILE__);

		check_point(1);
		ercd = dis_dsp();
		check_ercd(ercd, E_OK);

		while(fetch_pending_twdswitch() == false);;

		check_point(2);
		ercd = ena_dsp();
		check_ercd(ercd, E_OK);

		check_point(7);
		ercd = chg_ipm(TMAX_INTPRI);
		check_ercd(ercd, E_OK);

		while(fetch_pending_twdswitch() == false);;

		check_point(8);
		ercd = chg_ipm(TIPM_ENAALL);
		check_ercd(ercd, E_OK);

		check_point(13);
		ercd = act_tsk(TASK1);
		check_ercd(ercd, E_OK);

		ercd = dis_dsp();
		check_ercd(ercd, E_OK);

		while(fetch_pending_twdswitch() == false);;

		check_point(14);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_assert(false);

	case 2:
		check_point(19);
		ercd = act_tsk(TASK4);
		check_ercd(ercd, E_OK);

		check_point(20);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_assert(false);

	case 3:
		check_point(24);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_assert(false);

	case 4:
		check_point(28);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_assert(false);

	case 5:
		check_finish(32);
		check_assert(false);

	default:
		check_assert(false);
	}
	check_assert(false);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(3);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	while(fetch_pending_twdswitch() == false);;

	check_point(4);
	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(9);
	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	while(fetch_pending_twdswitch() == false);;

	check_point(10);
	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	check_point(15);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	while(fetch_pending_twdswitch() == false);;

	check_point(16);
	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_assert(false);
}

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(5);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	while(fetch_pending_scycswitch() == false);;

	check_point(6);
	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(11);
	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	while(fetch_pending_scycswitch() == false);;

	check_point(12);
	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	check_point(17);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	while(fetch_pending_scycswitch() == false);;

	check_point(18);
	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_assert(false);
}

void
task4(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(21);
	ercd = chg_som(TSOM_STP);
	check_ercd(ercd, E_OK);

	ercd = dly_tsk(21 * TEST_TIME_CP);
	check_ercd(ercd, E_OK);

	check_point(22);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = chg_som(SOM1);
	check_ercd(ercd, E_OK);

	check_assert(fetch_pending_scycswitch() == true);

	ercd = act_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(23);
	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(25);
	ercd = chg_som(TSOM_STP);
	check_ercd(ercd, E_OK);

	ercd = dly_tsk(21 * TEST_TIME_CP);
	check_ercd(ercd, E_OK);

	check_point(26);
	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	ercd = chg_som(SOM1);
	check_ercd(ercd, E_OK);

	check_assert(fetch_pending_scycswitch() == true);

	ercd = act_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(27);
	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	check_point(29);
	ercd = chg_som(TSOM_STP);
	check_ercd(ercd, E_OK);

	ercd = dly_tsk(21 * TEST_TIME_CP);
	check_ercd(ercd, E_OK);

	check_point(30);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = chg_som(SOM1);
	check_ercd(ercd, E_OK);

	check_assert(fetch_pending_scycswitch() == true);

	ercd = act_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(31);
	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_assert(false);
}
