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
 *  $Id: test_lmtdom2.c 457 2018-09-22 13:13:08Z ertl-hiro $
 */

/* 
 *		保護ドメインに対する制限に関するテスト(2)
 *
 * 【テストの目的】
 *
 *  保護ドメインに対する制限と，保護ドメインのアクセス許可ベクタにより，
 *  ミューテックスをロックするサービスコール（loc_mtx，ploc_mtx，
 *  tloc_mtx）が発行可能かどうかをテストする．
 *
 * 【テスト項目】
 *
 *	(A) デフォルトの保護ドメインに属するタスクに関するテスト
 *		(A-1) 優先度上限がTMIN_TPRIのミューテックスに対するloc_mtxがエ
 *		      ラーになること
 *		(A-2) 優先度上限がTMIN_TPRI+1のミューテックスに対するloc_mtxが
 *		      実行できること
 *		(A-3) 優先度上限がTMIN_TPRIのミューテックスに対するploc_mtxが
 *		      エラーになること
 *		(A-4) 優先度上限がTMIN_TPRIのミューテックスに対するtloc_mtxが
 *		      エラーになること
 *	(B) アクセス許可ベクタがデフォルトで，指定できる最高のタスク優先度
 *		がTMIN_TPRIの保護ドメインに属するタスクに関するテスト
 *		(B-1) 優先度上限がTMIN_TPRIのミューテックスに対するloc_mtxが実
 *		      行できること
 *		(B-2) 優先度上限がTMIN_TPRI+1のミューテックスに対するloc_mtxが
 *		      実行できること
 *	(C) 自保護ドメインから通常操作2が許可されており，設定できる最高の
 *		タスク優先度がデフォルトの保護ドメインに属するタスクに関するテ
 *		スト
 *		(C-1) 優先度上限がTMIN_TPRIのミューテックスに対するloc_mtxが実
 *		      行できること
 *		(C-2) 優先度上限がTMIN_TPRI+1のミューテックスに対するloc_mtxが
 *		      実行できること
 *	(D) カーネルドメイン（アクセス許可ベクタと設定できる最高のタスク優
 *		先度はデフォルト）に属するタスクに関するテスト
 *		(D-1) 優先度上限がTMIN_TPRIのミューテックスに対するloc_mtxが実
 *		      行できること
 *		(D-2) 優先度上限がTMIN_TPRI+1のミューテックスに対するloc_mtxが
 *		      実行できること
 *
 * 【使用リソース】
 *
 *	DOM1: アクセス許可ベクタ：デフォルト，設定できる最高のタスク優先度：
 *		  デフォルト
 *	DOM2: アクセス許可ベクタ：デフォルト，設定できる最高のタスク優先度：
 *		  TMIN_TPRI+1
 *	DOM3: アクセス許可ベクタ：DOM3からの通常操作2が許可，設定できる最
 *		  高のタスク優先度：デフォルト
 *	TASK1: DOM1，高優先度タスク
 *	TASK2: DOM2，高優先度タスク
 *	TASK3: DOM3，高優先度タスク
 *	TASK4: カーネルドメイン，中優先度タスク，TA_ACT属性
 *	MTX1: 無所属，優先度上限，上限優先度: TMIN_TPRI
 *	MTX2: 無所属，優先度上限，上限優先度: TMIN_TPRI+1
 *
 * 【テストシーケンス】
 *
 *	== TASK4 ==
 *	1:	loc_mtx(MTX1)											... (D-1)
 *		unl_mtx(MTX1)
 *	2:	loc_mtx(MTX2)											... (D-2)
 *		unl_mtx(MTX2)
 *		act_tsk(TASK1)
 *	== TASK1 ==
 *	3:	loc_mtx(MTX1) -> E_OACV									... (A-1)
 *		ploc_mtx(MTX1) -> E_OACV								... (A-3)
 *		tloc_mtx(MTX1, 1000) -> E_OACV							... (A-4)
 *	4:	loc_mtx(MTX2)											... (A-2)
 *		unl_mtx(MTX2)
 *		ext_tsk()
 *	== TASK4（続き） ==
 *	5:	act_tsk(TASK2)
 *	== TASK2 ==
 *	6:	loc_mtx(MTX1)											... (B-1)
 *		unl_mtx(MTX1)
 *	7:	loc_mtx(MTX2)											... (B-2)
 *		unl_mtx(MTX2)
 *		ext_tsk()
 *	== TASK4（続き） ==
 *	8:	act_tsk(TASK3)
 *	== TASK3 ==
 *	9:	loc_mtx(MTX1)											... (C-1)
 *		unl_mtx(MTX1)
 *	10:	loc_mtx(MTX2)											... (C-2)
 *		unl_mtx(MTX2)
 *		ext_tsk()
 *	== TASK4（続き） ==
 *	11:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_lmtdom2.h"

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(3);
	ercd = loc_mtx(MTX1);
	check_ercd(ercd, E_OACV);

	check_point(4);
	ercd = loc_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = unl_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(6);
	ercd = loc_mtx(MTX1);
	check_ercd(ercd, E_OK);

	ercd = unl_mtx(MTX1);
	check_ercd(ercd, E_OK);

	check_point(7);
	ercd = loc_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = unl_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(9);
	ercd = loc_mtx(MTX1);
	check_ercd(ercd, E_OK);

	ercd = unl_mtx(MTX1);
	check_ercd(ercd, E_OK);

	check_point(10);
	ercd = loc_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = unl_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = ext_tsk();
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task4(intptr_t exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	ercd = loc_mtx(MTX1);
	check_ercd(ercd, E_OK);

	ercd = unl_mtx(MTX1);
	check_ercd(ercd, E_OK);

	check_point(2);
	ercd = loc_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = unl_mtx(MTX2);
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = act_tsk(TASK2);
	check_ercd(ercd, E_OK);

	check_point(8);
	ercd = act_tsk(TASK3);
	check_ercd(ercd, E_OK);

	check_finish(11);
	check_point(0);
}
