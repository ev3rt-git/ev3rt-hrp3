/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2015 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_lmtdom1.c 457 2018-09-22 13:13:08Z ertl-hiro $
 */

/* 
 *		保護ドメインに対する制限に関するテスト(1)
 *
 * 【テストの目的】
 *
 *  保護ドメインに対する制限と，保護ドメインのアクセス許可ベクタにより，
 *  優先度を変更するサービスコール（chg_pri）が発行可能かどうかを，網羅
 *  的にテストする．
 *
 * 【テスト項目】
 *
 *	(A) デフォルトの保護ドメインに属するデフォルトのタスクに関するテスト
 *		(A-1) 自保護ドメインからのchg_pri(TMIN_TPRI)がエラーになること
 *		(A-2) 自保護ドメインからのchg_pri(TMIN_TPRI+1)が実行できること
 *		(A-3) 他保護ドメインからのchg_priがエラーになること
 *		(A-4) カーネルドメインからのchg_pri(TMIN_TPRI)が実行できること
 *	(B) アクセス許可ベクタがデフォルトで，設定できる最高のタスク優先度
 *		がTMIN_TPRI+2の保護ドメインに属する，通常操作2がすべての保護ド
 *		メインに許可されたタスクに関するテスト
 *		(B-1) 自保護ドメインからのchg_pri(TMIN_TPRI+1)がエラーになること
 *		(B-2) 自保護ドメインからのchg_pri(TMIN_TPRI+2)が実行できること
 *		(B-3) 他保護ドメインからのchg_pri(TMIN_TPRI+1)がエラーになること
 *		(B-4) 他保護ドメインからのchg_pri(TMIN_TPRI+2)が実行できること
 *		(B-5) カーネルドメインからのchg_pri(TMIN_TPRI+1)が実行できること
 *	(C) 自保護ドメインAと他の1つの保護ドメインBから通常操作2が許可され
 *		ており，設定できる最高のタスク優先度がTMIN_TPRI+3の保護ドメイン
 *		に属する，通常操作2がすべての保護ドメイン許可されたタスクに関す
 *		るテスト
 *		(C-1) 自保護ドメインAからのchg_pri(TMIN_TPRI+2)が実行できること
 *		(C-2) 他保護ドメインBからのchg_pri(TMIN_TPRI+2)が実行できること
 *		(C-3) 他保護ドメインCからのchg_pri(TMIN_TPRI+2)がエラーになること
 *		(C-4) 他保護ドメインCからのchg_pri(TMIN_TPRI+3)が実行できること
 *		(C-5) カーネルドメインからのchg_pri(TMIN_TPRI+2)が実行できること
 *
 * 【使用リソース】
 *
 *	DOM1: アクセス許可ベクタ：デフォルト，設定できる最高のタスク優先度：
 *		  デフォルト
 *	DOM2: アクセス許可ベクタ：デフォルト，設定できる最高のタスク優先度：
 *		  TMIN_TPRI+1
 *	DOM3: アクセス許可ベクタ：DOM1とDOM3からの通常操作2が許可，設定でき
 *		  る最高のタスク優先度：デフォルト
 *	TASK1: DOM1，中優先度タスク，TA_ACT属性，アクセス許可ベクタ：デフォルト
 *	TASK2: DOM2，中優先度タスク，通常操作1，通常操作2，参照操作がすべての
 *				 保護ドメインに許可
 *	TASK3: DOM3，中優先度タスク，通常操作1，通常操作2，参照操作がすべての
 *				 保護ドメインに許可
 *	TASK4: カーネルドメイン，中優先度タスク，通常操作1と参照操作がすべて
 *		   の保護ドメインに許可
 *
 * 【テストシーケンス】
 *
 *	== TASK1 ==
 *	1:	chg_pri(TASK1, TMIN_TPRI) -> E_OACV						... (A-1)
 *	2:	chg_pri(TASK1, TMIN_TPRI + 1)							... (A-2)
 *		act_tsk(TASK2)
 *		chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK2 ==
 *	3:	chg_pri(TASK1, TMAX_TPRI) -> E_OACV						... (A-3)
 *		act_tsk(TASK4)
 *	== TASK4-1 ==
 *	4:	chg_pri(TASK1, TMIN_TPRI)								... (A-4)
 *		ext_tsk()
 *	== TASK1（続き） ==
 *	5:	chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK2（続き）==
 *	6:	chg_pri(TASK2, TMIN_TPRI + 1) -> E_OACV					... (B-1)
 *	7:	chg_pri(TASK2, TMIN_TPRI + 2)							... (B-2)
 *		chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK1（続き） ==
 *	8:	chg_pri(TASK2, TMIN_TPRI + 1) -> E_OACV					... (B-3)
 *	9:	chg_pri(TASK2, TMIN_TPRI + 2)							... (B-4)
 *	== TASK2（続き）==
 *	10:	act_tsk(TASK4)
 *		chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK4-2 ==
 *	11:	chg_pri(TASK2, TMIN_TPRI + 1)							... (B-5)
 *		ext_tsk()
 *	== TASK2（続き）==
 *	12:	act_tsk(TASK3)
 *		chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK1（続き） ==
 *	13:	chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK3 ==
 *	14: chg_pri(TASK3, TMIN_TPRI + 2)							... (C-1)
 *		chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK2（続き）==
 *	15: chg_pri(TASK3, TMIN_TPRI + 2)							... (C-2)
 *	== TASK3 ==
 *	16:	chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK2 ==
 *	17:	chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK1（続き） ==
 *	18: chg_pri(TASK3, TMIN_TPRI + 2) -> E_OACV					... (C-3)
 *	19: chg_pri(TASK3, TMIN_TPRI + 3)							... (C-4)
 *	== TASK3 ==
 *	20:	act_tsk(TASK4)
 *		chg_pri(TSK_SELF, TPRI_INI)
 *	== TASK4-3 ==
 *	21: chg_pri(TASK3, TMIN_TPRI + 2)							... (C-5)
 *		ext_tsk()
 *	== TASK3（続き）==
 *	22:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_lmtdom1.h"

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	ercd = chg_pri(TASK1, TMIN_TPRI);
	check_ercd(ercd, E_OACV);

	check_point(2);
	ercd = chg_pri(TASK1, TMIN_TPRI + 1);
	check_ercd(ercd, E_OK);

	ercd = act_tsk(TASK2);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(8);
	ercd = chg_pri(TASK2, TMIN_TPRI + 1);
	check_ercd(ercd, E_OACV);

	check_point(9);
	ercd = chg_pri(TASK2, TMIN_TPRI + 2);
	check_ercd(ercd, E_OK);

	check_point(13);
	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(18);
	ercd = chg_pri(TASK3, TMIN_TPRI + 2);
	check_ercd(ercd, E_OACV);

	check_point(19);
	ercd = chg_pri(TASK3, TMIN_TPRI + 3);
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(3);
	ercd = chg_pri(TASK1, TMAX_TPRI);
	check_ercd(ercd, E_OACV);

	ercd = act_tsk(TASK4);
	check_ercd(ercd, E_OK);

	check_point(6);
	ercd = chg_pri(TASK2, TMIN_TPRI + 1);
	check_ercd(ercd, E_OACV);

	check_point(7);
	ercd = chg_pri(TASK2, TMIN_TPRI + 2);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(10);
	ercd = act_tsk(TASK4);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(12);
	ercd = act_tsk(TASK3);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(15);
	ercd = chg_pri(TASK3, TMIN_TPRI + 2);
	check_ercd(ercd, E_OK);

	check_point(17);
	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;

	check_point(14);
	ercd = chg_pri(TASK3, TMIN_TPRI + 2);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(16);
	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_point(20);
	ercd = act_tsk(TASK4);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TSK_SELF, TPRI_INI);
	check_ercd(ercd, E_OK);

	check_finish(22);
	check_point(0);
}

static uint_t	task4_count = 0;

void
task4(intptr_t exinf)
{
	ER_UINT	ercd;

	switch (++task4_count) {
	case 1:
		check_point(4);
		ercd = chg_pri(TASK1, TMIN_TPRI);
		check_ercd(ercd, E_OK);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 2:
		check_point(11);
		ercd = chg_pri(TASK2, TMIN_TPRI + 1);
		check_ercd(ercd, E_OK);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	case 3:
		check_point(21);
		ercd = chg_pri(TASK3, TMIN_TPRI + 2);
		check_ercd(ercd, E_OK);

		ercd = ext_tsk();
		check_ercd(ercd, E_OK);

		check_point(0);

	default:
		check_point(0);
	}
	check_point(0);
}
