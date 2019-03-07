/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_ovrhdr5.c 598 2018-12-12 00:33:36Z ertl-hiro $
 */

/* 
 *		オーバランハンドラ機能のテスト(5)
 *
 * 【テストの目的】
 *
 *	オーバランハンドラの機能仕様に関して，HRP3カーネルのみに適用される
 *	仕様をテストする．
 *
 * 【テスト項目】
 *
 *	test_ovrhdr.txtを参照すること．
 *
 * 【使用リソース】
 *
 *	TASK1: DOM1，中優先度タスク，メインタスク，最初から起動
 *	TASK2: DOM2，高優先度タスク，DOM1から通常操作1,2が可能
 *	TASK3: DOM3，高優先度タスク，DOM1から参照操作が可能
 *	OVR:   オーバランハンドラ
 *	EXTSVC1: 拡張サービスコール
 *
 * 【テストシーケンス】
 *
 *	== TASK1（優先度：中）==
 *	1:	stp_ovr(TASK2)										...［NGKI2651_T2］
 *		sta_ovr(TASK2, UNIT_TIME)							...［NGKI2637_T2］
 *		ref_ovr(TASK2, &rovr) -> E_OACV						...［NGKI2661_T1］
 *		stp_ovr(TASK3) -> E_OACV							...［NGKI2651_T1］
 *		sta_ovr(TASK3, UNIT_TIME) -> E_OACV					...［NGKI2637_T1］
 *		ref_ovr(TASK3, &rovr)								...［NGKI2661_T2］
 *															...［NGKI2662_T2］
 *		assert(rovr.ovrstat == TOVR_STP)
 *		DO(p_rovr_DOM1 = &rovr)
 *		act_tsk(TASK2)
 *	== TASK2（優先度：高）==
 *	2:	ref_ovr(TSK_SELF, &rovr)
 *		assert(rovr.ovrstat == TOVR_STA)
 *		assert(0 < rovr.leftotm && rovr.leftotm < UNIT_TIME)
 *		_ref_ovr(TSK_SELF, p_rovr_DOM1) -> E_MACV			...［NGKI2662_T1］
 *		cal_svc(TFN_EXTSVC1, 0, 0, 0, 0, 0)
 *	== EXTSVC1 ==
 *	3:	DO(while(!task2_flag1))			// ここで残りの時間を消費
 *	== OVR ==
 *	4:	assert(tskid == TASK2)								...［NGKI2590_T2］
 *		assert(exinf == 2)
 *		loc_cpu()											...［NGKI2597_T1］
 *		unl_cpu()
 *		DO(task2_flag1 = true)
 *		RETURN
 *	== EXTSVC1（続き）==
 *	5:	RETURN(E_OK)
 *	== TASK2（続き）==
 *	6:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_ovrhdr5.h"

#define _ref_ovr		ref_ovr
T_ROVR	*p_rovr_DOM1;

volatile bool_t	task2_flag1 = false;

void
task3(intptr_t exinf)
{
	check_point(0);
}

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

ER_UINT
extsvc1_routine(intptr_t par1, intptr_t par2, intptr_t par3,
								intptr_t par4, intptr_t par5, ID cdmid)
{

	check_point(3);
	while(!task2_flag1);

	check_point(5);
	return(E_OK);

	check_point(0);
	return(E_SYS);
}

void
overrun_handler(ID tskid, intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(4);
	check_assert(tskid == TASK2);

	check_assert(exinf == 2);

	ercd = loc_cpu();
	check_ercd(ercd, E_OK);

	ercd = unl_cpu();
	check_ercd(ercd, E_OK);

	task2_flag1 = true;

	return;

	check_point(0);
}

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;
	T_ROVR	rovr;

	test_start(__FILE__);

	check_point(1);
	ercd = stp_ovr(TASK2);
	check_ercd(ercd, E_OK);

	ercd = sta_ovr(TASK2, UNIT_TIME);
	check_ercd(ercd, E_OK);

	ercd = ref_ovr(TASK2, &rovr);
	check_ercd(ercd, E_OACV);

	ercd = stp_ovr(TASK3);
	check_ercd(ercd, E_OACV);

	ercd = sta_ovr(TASK3, UNIT_TIME);
	check_ercd(ercd, E_OACV);

	ercd = ref_ovr(TASK3, &rovr);
	check_ercd(ercd, E_OK);

	check_assert(rovr.ovrstat == TOVR_STP);

	p_rovr_DOM1 = &rovr;

	ercd = act_tsk(TASK2);
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;
	T_ROVR	rovr;

	check_point(2);
	ercd = ref_ovr(TSK_SELF, &rovr);
	check_ercd(ercd, E_OK);

	check_assert(rovr.ovrstat == TOVR_STA);

	check_assert(0 < rovr.leftotm && rovr.leftotm < UNIT_TIME);

	ercd = _ref_ovr(TSK_SELF, p_rovr_DOM1);
	check_ercd(ercd, E_MACV);

	ercd = cal_svc(TFN_EXTSVC1, 0, 0, 0, 0, 0);
	check_ercd(ercd, E_OK);

	check_finish(6);
	check_point(0);
}
