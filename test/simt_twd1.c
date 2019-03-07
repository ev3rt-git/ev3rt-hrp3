/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2014-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: simt_twd1.c 586 2018-12-02 09:31:05Z ertl-hiro $
 */

/*
 *		タイムウィンドウ管理機能のテスト(1)
 *
 * 【テストの目的】
 *
 *  タイムウィンドウ／アイドルウィンドウに与えられる時間についてテスト
 *  する．
 *
 * 【使用リソース】
 *
 *	高分解能タイマモジュールの性質：HRT_CONFIG1
 *		TCYC_HRTCNT		未定義（2^32の意味）
 *		TSTEP_HRTCNT	1U
 *		HRTCNT_BOUND	4000000002U
 *
 *	タイマドライバシミュレータのパラメータ
 *		SIMTIM_INIT_CURRENT		10
 *		SIMTIM_OVERHEAD_HRTINT	10
 *
 *	システム周期: 1000
 *	DOM1: システム周期の最初タイムウィンドウ（長さ: 500）を割当て
 *	DOM2: タイムウィンドウを割り当てない
 *	TASK1: DOM1，中優先度，TA_ACT属性
 *	TASK2: DOM2，中優先度，TA_ACT属性
 *	TASK3: カーネルドメイン，中優先度
 *	ALM1: カーネルドメイン
 *
 * 【テストシーケンス】
 *
 *	== HRT_HANDLER ==										// 時刻：10
 *	//	時刻が10進む
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	1:	assert(fch_hrt() == 20U)							// 時刻：20
 *	2:	sta_alm(ALM1, 200U)									// 発生：221
 *		DO(simtim_advance(499U))
 *	== HRT_HANDLER ==										// 時刻：221
 *	//	時刻が10進む
 *	== ALM1-1（1回目）==
 *	3:	assert(fch_hrt() == 231U)							// 時刻：231
 *		DO(simtim_advance(10U))
 *		assert(fch_hrt() == 241U)							// 時刻：241
 *		RETURN
 *	== TASK1（続き）==
 *	4:	assert(fch_hrt() == 539U)							// 時刻：539
 *		DO(simtim_advance(1U))
 *	== TWD_HANDLER ==										// 時刻：540
 *	//	時刻が10進む
 *	// アイドルウィンドウ
 *	== TASK2 ==
 *	5:	assert(fch_hrt() == 550U)							// 時刻：550
 *	6:	sta_alm(ALM1, 200U)									// 発生：751
 *		DO(simtim_advance(439U))
 *	== HRT_HANDLER ==										// 時刻：751
 *	//	時刻が10進む
 *	== ALM1-2（2回目）==
 *	7:	assert(fch_hrt() == 761U)							// 時刻：761
 *		DO(simtim_advance(10U))
 *		assert(fch_hrt() == 771U)							// 時刻：771
 *		RETURN
 *	== TASK2（続き）==
 *	8:	assert(fch_hrt() == 1009U)							// 時刻：1009
 *		DO(simtim_advance(1U))
 *	== HRT_HANDLER ==										// 時刻：1010
 *	//	時刻が10進む
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	9:	assert(fch_hrt() == 1020U)							// 時刻：1020
 *	10:	sta_alm(ALM1, 200U)									// 発生：1221
 *		DO(simtim_advance(499U))
 *	== HRT_HANDLER ==										// 時刻：1221
 *	//	時刻が10進む
 *	== ALM1-3（3回目）==
 *	11:	assert(fch_hrt() == 1231U)							// 時刻：1231
 *		DO(simtim_advance(10U))
 *		assert(fch_hrt() == 1241U)							// 時刻：1241
 *	12:	act_tsk(TASK3)
 *		RETURN
 *	== TASK3-1（1回目）==
 *	13:	assert(fch_hrt() == 1241U)							// 時刻：1241
 *		DO(simtim_advance(20U))
 *		assert(fch_hrt() == 1261U)							// 時刻：1261
 *		RETURN
 *	== TASK1（続き）==
 *	14:	assert(fch_hrt() == 1559U)							// 時刻：1559
 *		DO(simtim_advance(1U))
 *	== TWD_HANDLER ==										// 時刻：1560
 *	//	時刻が10進む
 *	// アイドルウィンドウ
 *	== TASK2 ==
 *	15:	assert(fch_hrt() == 1570U)							// 時刻：1570
 *	16:	sta_alm(ALM1, 200U)									// 発生：1771
 *		DO(simtim_advance(399U))
 *	== HRT_HANDLER ==										// 時刻：1771
 *	//	時刻が10進む
 *	== ALM1-4（4回目）==
 *	17:	assert(fch_hrt() == 1781U)							// 時刻：1781
 *		DO(simtim_advance(10U))
 *		assert(fch_hrt() == 1791U)							// 時刻：1791
 *	18:	act_tsk(TASK3)
 *		RETURN
 *	== TASK3-2（2回目）==
 *	19:	assert(fch_hrt() == 1791U)							// 時刻：1791
 *		DO(simtim_advance(20U))
 *		assert(fch_hrt() == 1811U)							// 時刻：1811
 *		RETURN
 *	== TASK2（続き）==
 *	20:	assert(fch_hrt() == 2009U)							// 時刻：2009
 *		DO(simtim_advance(1U))
 *	== HRT_HANDLER ==										// 時刻：2010
 *	//	時刻が10進む
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	21:	assert(fch_hrt() == 2020U)							// 時刻：2020
 *	22:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "arch/simtimer/sim_timer_cntl.h"
#include "kernel_cfg.h"
#include "simt_twd1.h"

#ifndef HRT_CONFIG1
#error Compiler option "-DHRT_CONFIG1" is missing.
#endif /* HRT_CONFIG1 */

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

static uint_t	alarm1_count = 0;

void
alarm1_handler(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++alarm1_count) {
	case 1:
		check_point(3);
		check_assert(fch_hrt() == 231U);

		simtim_advance(10U);

		check_assert(fch_hrt() == 241U);

		return;

		check_point(0);

	case 2:
		check_point(7);
		check_assert(fch_hrt() == 761U);

		simtim_advance(10U);

		check_assert(fch_hrt() == 771U);

		return;

		check_point(0);

	case 3:
		check_point(11);
		check_assert(fch_hrt() == 1231U);

		simtim_advance(10U);

		check_assert(fch_hrt() == 1241U);

		check_point(12);
		ercd = act_tsk(TASK3);
		check_ercd(ercd, E_OK);

		return;

		check_point(0);

	case 4:
		check_point(17);
		check_assert(fch_hrt() == 1781U);

		simtim_advance(10U);

		check_assert(fch_hrt() == 1791U);

		check_point(18);
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
HRT_HANDLER(void)
{

	test_start(__FILE__);

	check_point(0);
}

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(1);
	check_assert(fch_hrt() == 20U);

	check_point(2);
	ercd = sta_alm(ALM1, 200U);
	check_ercd(ercd, E_OK);

	simtim_advance(499U);

	check_point(4);
	check_assert(fch_hrt() == 539U);

	simtim_advance(1U);

	check_point(9);
	check_assert(fch_hrt() == 1020U);

	check_point(10);
	ercd = sta_alm(ALM1, 200U);
	check_ercd(ercd, E_OK);

	simtim_advance(499U);

	check_point(14);
	check_assert(fch_hrt() == 1559U);

	simtim_advance(1U);

	check_point(21);
	check_assert(fch_hrt() == 2020U);

	check_finish(22);
	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(5);
	check_assert(fch_hrt() == 550U);

	check_point(6);
	ercd = sta_alm(ALM1, 200U);
	check_ercd(ercd, E_OK);

	simtim_advance(439U);

	check_point(8);
	check_assert(fch_hrt() == 1009U);

	simtim_advance(1U);

	check_point(15);
	check_assert(fch_hrt() == 1570U);

	check_point(16);
	ercd = sta_alm(ALM1, 200U);
	check_ercd(ercd, E_OK);

	simtim_advance(399U);

	check_point(20);
	check_assert(fch_hrt() == 2009U);

	simtim_advance(1U);

	check_point(0);
}

static uint_t	task3_count = 0;

void
task3(intptr_t exinf)
{

	switch (++task3_count) {
	case 1:
		check_point(13);
		check_assert(fch_hrt() == 1241U);

		simtim_advance(20U);

		check_assert(fch_hrt() == 1261U);

		return;

		check_point(0);

	case 2:
		check_point(19);
		check_assert(fch_hrt() == 1791U);

		simtim_advance(20U);

		check_assert(fch_hrt() == 1811U);

		return;

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}
