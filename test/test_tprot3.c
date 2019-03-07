/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2015-2019 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_tprot3.c 634 2019-01-04 10:24:40Z ertl-hiro $
 */

/* 
 *		時間パーティショニングに関するテスト(3)
 *
 * 【テストの目的】
 *
 *  時間パーティショニングを使用した時のシステム動作モードの切換えに関
 *  して，様々な状況を網羅的にテストする．
 *
 * 【テスト項目】
 *
 *	(A) システム動作モードの自動切換え
 *		(A-1) 同じシステム動作モードへの切換え
 *		(A-2) 異なるシステム動作モードへの切換え
 *	(B) 通常のシステム動作モードからのchg_som
 *		(B-1) 通常のシステム動作モードへの切換え
 *		(B-2) システム周期停止モードへの切換え
 *	(C) システム周期停止モードからのchg_som
 *		(C-1) ディスパッチ保留状態でなく，タスク切換えが起こる時
 *		(C-2) ディスパッチ保留状態でなく，タスク切換えが起こらない時
 *		(C-3) ディスパッチ保留状態の時
 *
 * 【使用リソース】
 *
 *	システム周期: (20 * TEST_TIME_CP)
 *	DOM1: SOM1,SOM2,SOM3でタイムウィンドウを割当て
 *	DOM2: SOM1,SOM2でタイムウィンドウを割当て
 *	DOM3: タイムウィンドウを割り当てない
 *	SOM1: 初期モード，遷移先システム動作モード: SOM1，DOM2→DOM1→アイドル
 *	SOM2: 遷移先システム動作モード: SOM3，DOM1→DOM2→アイドル
 *	SOM3: 遷移先システム動作モード: SOM4，DOM1→アイドル
 *	SOM4: 遷移先システム動作モード: SOM2，アイドル
 *	TASK1: DOM1，中優先度，TA_ACT属性
 *	TASK2: DOM2，中優先度，TA_ACT属性
 *	TASK3: DOM3，中優先度，TA_ACT属性
 *	TASK4: カーネルドメイン，低優先度，TA_ACT属性
 *	CYC1: カーネルドメイン，システム周期毎にtask[123]_flagをセット
 *
 * 【テストシーケンス】
 *
 *	// システム動作モード：SOM1
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	1:	get_som(&somid)
 *		assert(somid == SOM1)
 *		DO(WAIT(task2_flag))
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	2:	DO(WAIT(task1_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	3:	DO(WAIT(task3_flag))
 *	// システム動作モード：SOM1										... (A-1)
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	4:	chg_som(SOM2)												... (B-1-1)
 *		get_som(&somid)
 *		assert(somid == SOM1)
 *		DO(WAIT(task2_flag))
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	5:	DO(WAIT(task1_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	6:	DO(WAIT(task3_flag))
 *	// システム動作モード：SOM2										... (B-1-2)
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	7:	get_som(&somid)
 *		assert(somid == SOM2)
 *		DO(WAIT(task1_flag))
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	8:	DO(WAIT(task2_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	9:	DO(WAIT(task3_flag))
 *	// システム動作モード：SOM3										... (A-2)
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	10:	get_som(&somid)
 *		assert(somid == SOM3)
 *		DO(WAIT(task1_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	11:	DO(WAIT(task3_flag))
 *	// システム動作モード：SOM4										... (A-2)
 *	== TASK3 ==
 *	12:	DO(WAIT(task3_flag))
 *	// システム動作モード：SOM2										... (A-2)
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	13:	get_som(&somid)
 *		assert(somid == SOM2)
 *		DO(WAIT(task1_flag))
 *	== TASK2 ==
 *	14:	DO(WAIT(task2_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	15:	chg_som(TSOM_STP)											... (B-2-1)
 *		get_som(&somid)
 *		assert(somid == SOM2)
 *	16:	DO(WAIT(task3_flag))
 *	// システム動作モード：TSOM_STP									... (B-2-2)
 *	== TASK4 ==
 *	17:	get_som(&somid)
 *		assert(somid == TSOM_STP)
 *		stp_cyc(CYC1)
 *		dly_tsk(TEST_TIME_PROC) ... TASK4が実行再開するまで
 *	// 実行すべきタスクがない
 *	== TASK4 ==
 *	18:	chg_som(SOM1)												... (C-1)
 *	// システム動作モード：SOM1
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	19:	sta_cyc(CYC1)
 *		get_som(&somid)
 *		assert(somid == SOM1)
 *		DO(WAIT(task2_flag))
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	20:	DO(WAIT(task1_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	21:	chg_som(TSOM_STP)
 *		DO(WAIT(task3_flag))
 *	// システム動作モード：TSOM_STP
 *	== TASK4 ==
 *	22:	get_som(&somid)
 *		assert(somid == TSOM_STP)
 *		stp_cyc(CYC1)
 *		chg_pri(TSK_SELF, HIGH_PRIORITY)
 *	23:	chg_som(SOM1)												... (C-2)
 *	// システム動作モード：SOM1
 *	// タイムウィンドウ for DOM2
 *	24:	sta_cyc(CYC1)
 *		get_som(&somid)
 *		assert(somid == SOM1)
 *		chg_pri(TSK_SELF, LOW_PRIORITY)
 *	== TASK2 ==
 *	25:	DO(WAIT(task2_flag))
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	26:	DO(WAIT(task1_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	27:	chg_som(TSOM_STP)
 *		DO(WAIT(task3_flag))
 *	// システム動作モード：TSOM_STP
 *	== TASK4 ==
 *	28:	get_som(&somid)
 *		assert(somid == TSOM_STP)
 *		stp_cyc(CYC1)
 *		dis_dsp()
 *	29:	chg_som(SOM1)												... (C-3)
 *		sta_cyc(CYC1)
 *		get_som(&somid)
 *		assert(check_somid(somid))
 *		ena_dsp()
 *	// システム動作モード：SOM1
 *	// タイムウィンドウ for DOM2
 *	== TASK2 ==
 *	30:	DO(WAIT(task2_flag))
 *	// タイムウィンドウ for DOM1
 *	== TASK1 ==
 *	31:	DO(WAIT(task1_flag))
 *	// アイドルウィンドウ
 *	== TASK3 ==
 *	32:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_tprot3.h"

volatile bool_t	task1_flag;
volatile bool_t	task2_flag;
volatile bool_t	task3_flag;

#define WAIT(flag)	do { (flag) = false; while (!(flag)); } while (false)
#define SET(flag)	do { (flag) = true; } while (false)

void
cyclic1_handler(intptr_t exinf)
{
	SET(task1_flag);
	SET(task2_flag);
	SET(task3_flag);
}

/*
 *  ディスパッチ保留状態でchg_somを呼び出して，システム周期停止モード
 *  （TSOM_STP）から他のモード（SOM1）に変更した直後に，get_somでシス
 *  テム動作モードを参照すると，マルチプロセッサ対応カーネルとそうでな
 *  いカーネルで，異なる結果になる可能性がある．
 *
 *  具体的には，マルチプロセッサ対応でないカーネルでは，システム周期の
 *  切り換えが保留されるため，TSOM_STPが参照される．それに対してマルチ
 *  プロセッサ対応カーネルでは，他のプロセッサで先にシステム周期の切り
 *  換え処理が行われると，一時的に切換え前のシステム動作モードで実行し
 *  ている状況［NGKI0636］が生じ，切換え後のシステム動作モード（SOM1）
 *  が参照される可能性がある［NGKI5135］．
 */
static bool_t
check_somid(ID somid)
{
#ifndef TOPPERS_SUPPORT_MULTI_PRC
	return(somid == TSOM_STP);
#else /* TOPPERS_SUPPORT_MULTI_PRC */
	return(somid == TSOM_STP || somid == SOM1);
#endif /* TOPPERS_SUPPORT_MULTI_PRC */
}

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;
	ID		somid;

	check_point(2);
	WAIT(task1_flag);

	check_point(5);
	WAIT(task1_flag);

	check_point(7);
	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM2);

	WAIT(task1_flag);

	check_point(10);
	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM3);

	WAIT(task1_flag);

	check_point(13);
	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM2);

	WAIT(task1_flag);

	check_point(20);
	WAIT(task1_flag);

	check_point(26);
	WAIT(task1_flag);

	check_point(31);
	WAIT(task1_flag);

	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;
	ID		somid;

	test_start(__FILE__);

	check_point(1);
	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM1);

	WAIT(task2_flag);

	check_point(4);
	ercd = chg_som(SOM2);
	check_ercd(ercd, E_OK);

	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM1);

	WAIT(task2_flag);

	check_point(8);
	WAIT(task2_flag);

	check_point(14);
	WAIT(task2_flag);

	check_point(19);
	ercd = sta_cyc(CYC1);
	check_ercd(ercd, E_OK);

	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM1);

	WAIT(task2_flag);

	check_point(25);
	WAIT(task2_flag);

	check_point(30);
	WAIT(task2_flag);

	check_point(0);
}

void
task3(intptr_t exinf)
{
	ER_UINT	ercd;
	ID		somid;

	check_point(3);
	WAIT(task3_flag);

	check_point(6);
	WAIT(task3_flag);

	check_point(9);
	WAIT(task3_flag);

	check_point(11);
	WAIT(task3_flag);

	check_point(12);
	WAIT(task3_flag);

	check_point(15);
	ercd = chg_som(TSOM_STP);
	check_ercd(ercd, E_OK);

	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM2);

	check_point(16);
	WAIT(task3_flag);

	check_point(21);
	ercd = chg_som(TSOM_STP);
	check_ercd(ercd, E_OK);

	WAIT(task3_flag);

	check_point(27);
	ercd = chg_som(TSOM_STP);
	check_ercd(ercd, E_OK);

	WAIT(task3_flag);

	check_finish(32);
	check_point(0);
}

void
task4(intptr_t exinf)
{
	ER_UINT	ercd;
	ID		somid;

	check_point(17);
	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == TSOM_STP);

	ercd = stp_cyc(CYC1);
	check_ercd(ercd, E_OK);

	ercd = dly_tsk(TEST_TIME_PROC);
	check_ercd(ercd, E_OK);

	check_point(18);
	ercd = chg_som(SOM1);
	check_ercd(ercd, E_OK);

	check_point(22);
	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == TSOM_STP);

	ercd = stp_cyc(CYC1);
	check_ercd(ercd, E_OK);

	ercd = chg_pri(TSK_SELF, HIGH_PRIORITY);
	check_ercd(ercd, E_OK);

	check_point(23);
	ercd = chg_som(SOM1);
	check_ercd(ercd, E_OK);

	check_point(24);
	ercd = sta_cyc(CYC1);
	check_ercd(ercd, E_OK);

	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == SOM1);

	ercd = chg_pri(TSK_SELF, LOW_PRIORITY);
	check_ercd(ercd, E_OK);

	check_point(28);
	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(somid == TSOM_STP);

	ercd = stp_cyc(CYC1);
	check_ercd(ercd, E_OK);

	ercd = dis_dsp();
	check_ercd(ercd, E_OK);

	check_point(29);
	ercd = chg_som(SOM1);
	check_ercd(ercd, E_OK);

	ercd = sta_cyc(CYC1);
	check_ercd(ercd, E_OK);

	ercd = get_som(&somid);
	check_ercd(ercd, E_OK);

	check_assert(check_somid(somid));

	ercd = ena_dsp();
	check_ercd(ercd, E_OK);

	check_point(0);
}
