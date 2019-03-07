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
 *  $Id: test_sched1.c 445 2018-08-17 16:22:40Z ertl-hiro $
 */

/* 
 *		ドメイン間のスケジューリングに関するテスト(1)
 *
 * 【テストの目的】
 *
 *  時間パーティショニングを使用しない場合の異なる保護ドメインに属する
 *  タスク間でのスケジューリングに関してテストする．
 *
 * 【テスト項目】
 *
 *	(A) カーネルドメインとユーザドメイン間の優先順位
 *		(A-1) カーネルドメインに属するタスクの方が優先度が高い場合［NGKI0204］
 *		(A-2) ユーザドメインに属するタスクの方が優先度が高い場合［NGKI0204］
 *		(A-3) 優先度が同じで，カーネルドメインに属するタスクの方が先に
 *			  実行できる状態になった場合［NGKI0588］
 *		(A-4) 優先度が同じで，ユーザドメインに属するタスクの方が先に実
 *			  行できる状態になった場合［NGKI0588］
 *	(B) 異なるユーザドメイン（ドメイン1とドメイン2）間の優先順位
 *		(B-1) ドメイン1に属するタスクの方が優先度が高い場合［NGKI0204］
 *		(B-2) ドメイン2に属するタスクの方が優先度が高い場合［NGKI0204］
 *		(B-3) 優先度が同じで，ドメイン1に属するタスクの方が先に実行でき
 *			  る状態になった場合［NGKI0205］［NGKI0589］
 *		(B-4) 優先度が同じで，ドメイン2に属するタスクの方が先に実行でき
 *			  る状態になった場合［NGKI0205］［NGKI0589］
 *
 * 【使用リソース】
 *
 *	DOM1: ユーザドメイン1
 *	DOM2: ユーザドメイン2
 *	TASK1: カーネルドメイン，中優先度，最初から起動
 *	TASK2: カーネルドメイン，高優先度
 *	TASK3: DOM1，中優先度
 *	TASK4: DOM1，高優先度
 *	TASK5: DOM2，中優先度
 *	TASK6: DOM2，高優先度
 *
 * 【テストシーケンス】
 *
 *	== TASK1（中優先度）==
 *	1:	act_tsk(TASK3)						... (A-3)
 *		act_tsk(TASK4)						... (A-2)
 *	== TASK4（高優先度）==
 *	2:	act_tsk(TASK2)						... (A-4)
 *	== TASK2（高優先度）==
 *	3:	slp_tsk()
 *	== TASK4（続き）==
 *	4:	slp_tsk()
 *	== TASK1（続き）==
 *	5:	ext_tsk()
 *	== TASK3（中優先度）==
 *	6:	wup_tsk(TASK2)						... (A-1)
 *	== TASK2（続き）==
 *	7:	ext_tsk()
 *	== TASK3（続き）==
 *	8:	act_tsk(TASK5)						... (B-3)
 *		act_tsk(TASK6)						... (B-2)
 *	== TASK6（高優先度）==
 *	9:	wup_tsk(TASK4)						... (B-4)
 *		ext_tsk()
 *	== TASK4（続き）==
 *	10:	slp_tsk()
 *	== TASK3（続き）==
 *	11:	ext_tsk()
 *	== TASK5（中優先度）==
 *	12:	wup_tsk(TASK4)						... (B-1)
 *	== TASK4（続き）==
 *	13:	ext_tsk()
 *	== TASK5（続き）==
 *	14:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_sched1.h"

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	ercd = act_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK4);
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(3);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(7);
	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(6);
	ercd = wup_tsk(TASK2);
	check_ercd(ercd, E_OK);

	check_point(8);
	ercd = act_tsk(TASK5);
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK6);
	check_ercd(ercd, E_OK);

	check_point(11);
	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task4(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(2);
	ercd = act_tsk(TASK2);
	check_ercd(ercd, E_OK);

	check_point(4);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(10);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(13);
	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task5(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(12);
	ercd = wup_tsk(TASK4);
	check_ercd(ercd, E_OK);

	check_finish(14);
	check_point(0);
}

void
task6(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(9);
	ercd = wup_tsk(TASK4);
	check_ercd(ercd, E_OK);

	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}
