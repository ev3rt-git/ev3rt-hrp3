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
 *  $Id: test_sysstat2.c 266 2018-03-04 08:40:05Z ertl-hiro $
 */

/* 
 *		システム状態に関するテスト(2)
 *
 * 【テストの目的】
 *
 *  保護機能対応カーネルにおける割込み優先度マスク，ディスパッチ禁止フ
 *  ラグ，ディスパッチ保留状態について，ホワイトボックス的にテストする．
 *
 * 【テスト項目】
 *
 *	(A) ディスパッチ禁止状態でタスク切換えが起きないこと
 *		(A-1) 同じ保護ドメイン内の他のタスクに切り換わらないこと
 *		(A-2) 他の保護ドメイン内のタスクに切り換わらないこと
 *	(B) ディスパッチの許可
 *		(B-1) 割込み優先度マスク全解除で，タスクが切り換わること
 *		(B-2) 割込み優先度マスク全解除でなく，タスクが切り換わらないこと
 *	(C) 割込み優先度マスクが全解除でない状態でタスク切換えが起きないこと
 *		(C-1) 同じ保護ドメイン内の他のタスクに切り換わらないこと
 *		(C-2) 他の保護ドメイン内のタスクに切り換わらないこと
 *	(D) 割込み優先度マスクの全解除
 *		(D-1) ディスパッチ許可状態で，タスクが切り換わること
 *		(D-2) ディスパッチ禁止状態で，タスクが切り換わらないこと
 *	(E) make_non_runnable中の実行パスの網羅
 *		(E-1) queue_empty(p_queue) && p_schedcb->p_predtsk == p_tcb && dspflg
 *		(E-2) queue_empty(p_queue) && p_schedcb->p_predtsk == p_tcb && !dspflg
 *		(E-3) queue_empty(p_queue) && !(p_schedcb->p_predtsk == p_tcb)
 *		(E-4) !queue_empty(p_queue) && p_schedcb->p_predtsk == p_tcb && dspflg
 *		(E-5) !queue_empty(p_queue) && p_schedcb->p_predtsk == p_tcb && !dspflg
 *		(E-6) !queue_empty(p_queue) && !(p_schedcb->p_predtsk == p_tcb)
 *	(F) ena_dsp中の実行パスの網羅
 *		(F-1) t_get_ipm() == TIPM_ENAALL
 *							&& p_runtsk->raster && p_runtsk->enater
 *		(F-2) t_get_ipm() == TIPM_ENAALL && !(p_runtsk->raster
 *							&& p_runtsk->enater) && p_runtsk != p_schedtsk
 *		(F-3) t_get_ipm() == TIPM_ENAALL && !(p_runtsk->raster
 *							&& p_runtsk->enater) && !(p_runtsk != p_schedtsk)
 *		(F-4) !(t_get_ipm() == TIPM_ENAALL)
 *	(G) chg_ipm中の実行パスの網羅（システム周期切換えの保留を除く）
 *		(G-1) intpri == TIPM_ENAALL && enadsp
 *							&& p_runtsk->raster && p_runtsk->enater
 *		(G-2) intpri == TIPM_ENAALL && enadsp && !(p_runtsk->raster
 *							&& p_runtsk->enater) && p_runtsk != p_schedtsk
 *		(G-3) intpri == TIPM_ENAALL && enadsp && !(p_runtsk->raster
 *							&& p_runtsk->enater) && !(p_runtsk != p_schedtsk)
 *		(G-4) !(intpri == TIPM_ENAALL && enadsp)
 *
 * 【使用リソース】
 *
 *	システム周期: (40 * TEST_TIME_CP)
 *	DOM1: システム周期の最初タイムウィンドウ（長さ: 30 * TEST_TIME_CP）を割当て
 *	TASK11: DOM1，中優先度，TA_ACT属性
 *	TASK12: DOM1，高優先度
 *	TASK21: カーネルドメイン，中優先度
 *
 * 【テストシーケンス】
 *
 *	== TASK11 ==
 *	1:	state(false, false, false, false, false)
 *		dis_dsp()
 *		state(false, false, true, true, false)
 *		act_tsk(TASK12)											... (A-1)
 *	2:	ena_dsp()												... (B-1)(F-2)
 *	== TASK12-1 ==
 *	3:	state(false, false, false, false, false)
 *		ext_tsk()												... (E-1)
 *	== TASK11 ==
 *	4:	dis_dsp()
 *		act_tsk(TASK21)											... (A-2)
 *	5:	ena_dsp()												... (B-1)(F-2)
 *	== TASK21-1 ==
 *	6:	state(false, false, false, false, false)
 *		ext_tsk()												... (E-1)
 *	== TASK11 ==
 *	7:	state(false, false, false, false, false)
 *		chg_ipm(TMAX_INTPRI)									... (G-4)
 *		state(false, false, false, true, false)
 *		act_tsk(TASK12)											... (C-1)
 *	8:	chg_ipm(TIPM_ENAALL)									... (D-1)(G-2)
 *	== TASK12-2 ==
 *	9:	state(false, false, false, false, false)
 *		slp_tsk()												... (E-1)
 *	== TASK11 ==
 *	10:	chg_ipm(TMAX_INTPRI)									... (G-4)
 *		act_tsk(TASK21)											... (C-2)
 *	11:	chg_ipm(TIPM_ENAALL)									... (D-1)(G-2)
 *	== TASK21-2 ==
 *	12:	state(false, false, false, false, false)
 *		slp_tsk()												... (E-1)
 *	== TASK11 ==
 *	13:	dis_dsp()
 *		chg_ipm(TMAX_INTPRI)									... (G-4)
 *		wup_tsk(TASK12)
 *		wup_tsk(TASK21)
 *		ena_dsp()												... (B-2)(F-4)
 *	14:	dis_dsp()
 *		chg_ipm(TIPM_ENAALL)									... (D-2)(G-4)
 *	15:	ena_dsp()												... (B-1)(F-2)
 *	== TASK12-2 ==
 *	16:	ext_tsk()												... (E-1)
 *	== TASK21-2 ==
 *	17:	ext_tsk()												... (E-1)
 *	== TASK11 ==
 *	18:	dis_dsp()
 *		act_tsk(TASK12)
 *		sus_tsk(TASK12)											... (E-2)
 *		ena_dsp()												... (F-3)
 *		rsm_tsk(TASK12)
 *	== TASK12-3 ==
 *	19:	ext_tsk()												... (E-1)
 *	== TASK11 ==
 *	20:	chg_ipm(TMAX_INTPRI)									... (G-4)
 *		act_tsk(TASK12)
 *		chg_pri(TASK12, MID_PRIORITY)
 *		sus_tsk(TASK12)											... (E-6)
 *		chg_ipm(TIPM_ENAALL)									... (G-3)
 *		rsm_tsk(TASK12)
 *	21:	rot_rdq(TSK_SELF)
 *	== TASK12-4 ==
 *	22:	ext_tsk()												... (E-4)
 *	== TASK11 ==
 *	23:	dis_dsp()
 *		act_tsk(TASK12)
 *		chg_pri(TASK12, MID_PRIORITY)
 *		rot_rdq(MID_PRIORITY)
 *		sus_tsk(TASK12)											... (E-5)
 *		ena_dsp()												... (F-3)
 *		rsm_tsk(TASK12)
 *	24:	rot_rdq(MID_PRIORITY)
 *	== TASK12-5 ==
 *	25:	ext_tsk()												... (E-4)
 *	== TASK11 ==
 *	26:	dis_dsp()
 *		act_tsk(TASK12)
 *		chg_pri(TASK12, LOW_PRIORITY)
 *		sus_tsk(TASK12)											... (E-3)
 *		ena_dsp()												... (F-3)
 *		rsm_tsk(TASK12)
 *	27: slp_tsk()												... (E-1)
 *	== TASK12-6 ==
 *	28:	dis_ter()
 *		wup_tsk(TASK11)
 *	== TASK11 ==
 *	29:	ras_ter(TASK12)
 *		dly_tsk(TEST_TIME_CP) ... 次のシステム周期で待ち解除される
 *	== TASK12-6 ==
 *	30:	dis_dsp()
 *		ena_ter()
 *		ena_dsp()												... (F-1)
 *	== TASK11 ==
 *	31:	act_tsk(TASK12)
 *	== TASK12-7 ==
 *	32:	dis_ter()
 *		slp_tsk() -> E_RASTER
 *	== TASK11 ==
 *	33:	ras_ter(TASK12)
 *	== TASK12-7 ==
 *	34:	chg_ipm(TMAX_INTPRI)									... (G-4)
 *		ena_ter()
 *		chg_ipm(TIPM_ENAALL)									... (G-1)
 *	== TASK11 ==
 *	35:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_sysstat2.h"

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task11(intptr_t exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	check_state(false, false, false, false, false);

	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	check_state(false, false, true, true, false);

	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(2);
	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(4);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK21);
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(7);
	check_state(false, false, false, false, false);

	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	check_state(false, false, false, true, false);

	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(8);
	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	check_point(10);
	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK21);
	check_ercd(ercd, E_OK);

	check_point(11);
	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	check_point(13);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	ercd = wup_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = wup_tsk(TASK21);
	check_ercd(ercd, E_OK);

	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(14);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	check_point(15);
	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(18);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = sus_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	ercd = rsm_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(20);
	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TASK12, MID_PRIORITY);
	check_ercd(ercd, E_OK);

	ercd = sus_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	ercd = rsm_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(21);
	ercd = rot_rdq(TSK_SELF);
	check_ercd(ercd, E_OK);

	check_point(23);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TASK12, MID_PRIORITY);
	check_ercd(ercd, E_OK);

	ercd = rot_rdq(MID_PRIORITY);
	check_ercd(ercd, E_OK);

	ercd = sus_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	ercd = rsm_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(24);
	ercd = rot_rdq(MID_PRIORITY);
	check_ercd(ercd, E_OK);

	check_point(26);
	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TASK12, LOW_PRIORITY);
	check_ercd(ercd, E_OK);

	ercd = sus_tsk(TASK12);
	check_ercd(ercd, E_OK);

	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	ercd = rsm_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(27);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(29);
	ercd = ras_ter(TASK12);
	check_ercd(ercd, E_OK);

	ercd = dly_tsk(TEST_TIME_CP);
	check_ercd(ercd, E_OK);

	check_point(31);
	ercd = act_tsk(TASK12);
	check_ercd(ercd, E_OK);

	check_point(33);
	ercd = ras_ter(TASK12);
	check_ercd(ercd, E_OK);

	check_finish(35);
	check_point(0);
}

static uint_t	task12_count = 0;

void
task12(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++task12_count) {
	case 1:
		check_point(3);
		check_state(false, false, false, false, false);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 2:
		check_point(9);
		check_state(false, false, false, false, false);

		ercd = slp_tsk();
		check_ercd(ercd, E_OK);

		check_point(16);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 3:
		check_point(19);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 4:
		check_point(22);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 5:
		check_point(25);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 6:
		check_point(28);
		ercd = dis_ter();
		check_ercd(ercd, E_OK);

		ercd = wup_tsk(TASK11);
		check_ercd(ercd, E_OK);

		check_point(30);
		ercd = dis_dsp();
		check_ercd(ercd, E_OK);

		ercd = ena_ter();
		check_ercd(ercd, E_OK);

		ercd = ena_dsp();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 7:
		check_point(32);
		ercd = dis_ter();
		check_ercd(ercd, E_OK);

		ercd = slp_tsk();
		check_ercd(ercd, E_RASTER);

		check_point(34);
		ercd = chg_ipm(TMAX_INTPRI);
		check_ercd(ercd, E_OK);

		ercd = ena_ter();
		check_ercd(ercd, E_OK);

		ercd = chg_ipm(TIPM_ENAALL);
		check_ercd(ercd, E_OK);

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}

static uint_t	task21_count = 0;

void
task21(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++task21_count) {
	case 1:
		check_point(6);
		check_state(false, false, false, false, false);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 2:
		check_point(12);
		check_state(false, false, false, false, false);

		ercd = slp_tsk();
		check_ercd(ercd, E_OK);

		check_point(17);
		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}
