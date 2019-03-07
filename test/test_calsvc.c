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
 *  $Id: test_calsvc.c 523 2018-11-04 12:47:50Z ertl-hiro $
 */

/* 
 *		サービスコール呼出しに関するテスト
 *
 * 【テストの目的】
 *
 *  ソフトウェア割込みによるサービスコール呼出しをテストする．
 *
 *	・ユーザタスクから呼び出した場合と，システムタスクから呼び出した場
 *	  合をテストする．
 *  ・サービスコールへの各パラメータには，異なる値を入れてテストする．
 *
 * 【テスト項目】
 *
 *	以下の各テストを，ユーザタスクから呼び出した場合（X-U）と，システ
 *	ムタスクから呼び出した場合（X-S）で実施する．
 *
 *	(A) パラメータがないサービスコール … loc_cpuでテスト
 *	(B) パラメータが1つのサービスコール … chg_ipmでテスト
 *	(C) パラメータが2つのサービスコール … set_flgでテスト
 *	(D) パラメータが3つのサービスコール … mget_lodでテスト
 *	(E) パラメータが4つのサービスコール … wai_flgでテスト
 *	(F) パラメータが5つのサービスコール … twai_flgでテスト
 *	(G) パラメータが1つでSYSTIM型のサービスコール … set_timでテスト
 *	(H) サービスコールによりシステム状態が変更されること
 *		(H-1) loc_cpuによりCPUロック状態に遷移すること
 *		(H-2) unl_cpuによりCPUロック解除に遷移すること
 *		(H-3) chg_ipmにより割込み優先度マスクを変更できること
 *
 * 【使用リソース】
 *
 *	DOM1: ユーザドメイン，システム状態に対する通常操作2を許可，カーネ
 *		  ルドメインに対する参照操作を許可
 *	TASK1: DOM1に属するユーザタスク，高優先度（後半で低優先度に）
 *	TASK2: システムタスク，中優先度
 *	FLG1: DOM1に属するイベントフラグ
 *	FLG2: DOM1に属するイベントフラグ
 *
 * 【テストシーケンス】
 *
 *	== TASK1（優先度：高）==
 *	1:	assert(sns_loc() == false)
 *		loc_cpu()											... (A-U)(H-1-U)
 *		assert(sns_loc() == true)
 *		unl_cpu()											... (H-2-U)
 *		assert(sns_loc() == false)
 *	2:	get_ipm(&intpri)
 *		assert(intpri == TIPM_ENAALL)
 *		chg_ipm(TMAX_INTPRI)								... (B-U)(H-3-U)
 *		get_ipm(&intpri)
 *		assert(intpri == TMAX_INTPRI)
 *		chg_ipm(TIPM_ENAALL)								... (B-U)(H-3-U)
 *		get_ipm(&intpri)
 *		assert(intpri == TIPM_ENAALL)
 *	3:	ref_flg(FLG1, &pk_rflg)
 *		assert(pk_rflg.flgptn == 0U)
 *		set_flg(FLG1, 0x55U)								... (C-U)
 *		ref_flg(FLG1, &pk_rflg)
 *		assert(pk_rflg.flgptn == 0x55U)
 *		set_flg(FLG2, 0xaaU)								... (C-U)
 *		ref_flg(FLG2, &pk_rflg)
 *		assert(pk_rflg.flgptn == 0xaaU)
 *	4:	mget_lod(DOM1, HIGH_PRIORITY, &load1)				... (D-U)
 *		assert(load1 == 1)
 *		mget_lod(TDOM_KERNEL, MID_PRIORITY, &load2)			... (D-U)
 *		assert(load2 == 1)
 *	5:	wai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1)				... (E-U)
 *		assert(flgptn1 == 0x55U)
 *	6:	wai_flg(FLG2, 0xf0U, TWF_ANDW, &flgptn2)			... (E-U)
 *	== TASK2（優先度：中）==
 *	7:	set_flg(FLG2, 0x50U)
 *	== TASK1（続き）==
 *	8:	assert(flgptn2 == 0xfaU)
 *	9:	twai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1, TMO_POL)	... (F-U)
 *		assert(flgptn1 == 0x55U)
 *	10:	twai_flg(FLG1, 0x0aU, TWF_ORW, &flgptn1, TMO_POL) -> E_TMOUT ... (F-U)
 *	11:	twai_flg(FLG2, 0x0fU, TWF_ANDW, &flgptn2, TMO_FEVR)	... (F-U)
 *	== TASK2（続き）==
 *	12:	set_flg(FLG2, 0x05U)
 *	== TASK1（続き）==
 *	13:	assert(flgptn2 == 0xffU)
 *		clr_flg(FLG1, 0x0U)
 *		clr_flg(FLG2, 0x0U)
 *	14:	set_tim((2LLU << 32) + 100U)							... (G-U)
 *		get_tim(&systim)
 *		assert((2LLU << 32) + 100U <= systim \
 *				&& systim <= (2LLU << 32) + 100U + TEST_TIME_PROC)
 *	15:	set_tim((4LLU << 32) + 1000U)							... (G-U)
 *		get_tim(&systim)
 *		assert((4LLU << 32) + 1000U <= systim \
 *				&& systim <= (4LLU << 32) + 1000U + TEST_TIME_PROC)
 *		chg_pri(TSK_SELF, LOW_PRIORITY)
 *	== TASK2（続き）
 *	16:	assert(sns_loc() == false)
 *		loc_cpu()											... (A-S)(H-1-S)
 *		assert(sns_loc() == true)
 *		unl_cpu()											... (H-2-S)
 *		assert(sns_loc() == false)
 *	17:	get_ipm(&intpri)
 *		assert(intpri == TIPM_ENAALL)
 *		chg_ipm(TMAX_INTPRI)								... (B-S)(H-3-S)
 *		get_ipm(&intpri)
 *		assert(intpri == TMAX_INTPRI)
 *		chg_ipm(TIPM_ENAALL)								... (B-S)(H-3-S)
 *		get_ipm(&intpri)
 *		assert(intpri == TIPM_ENAALL)
 *	18:	ref_flg(FLG1, &pk_rflg)
 *		assert(pk_rflg.flgptn == 0U)
 *		set_flg(FLG1, 0x55U)								... (C-S)
 *		ref_flg(FLG1, &pk_rflg)
 *		assert(pk_rflg.flgptn == 0x55U)
 *		set_flg(FLG2, 0xaaU)								... (C-S)
 *		ref_flg(FLG2, &pk_rflg)
 *		assert(pk_rflg.flgptn == 0xaaU)
 *	19:	mget_lod(DOM1, LOW_PRIORITY, &load1)				... (D-S)
 *		assert(load1 == 1)
 *		mget_lod(TDOM_KERNEL, MID_PRIORITY, &load2)			... (D-S)
 *		assert(load2 == 1)
 *	20:	wai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1)				... (E-S)
 *		assert(flgptn1 == 0x55U)
 *	21:	wai_flg(FLG2, 0xf0U, TWF_ANDW, &flgptn2)			... (E-S)
 *	== TASK1（続き）==
 *	22:	set_flg(FLG2, 0x50U)
 *	== TASK2（続き）==
 *	23:	assert(flgptn2 == 0xfaU)
 *	24:	twai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1, TMO_POL)	... (F-S)
 *		assert(flgptn1 == 0x55U)
 *	25:	twai_flg(FLG1, 0x0aU, TWF_ORW, &flgptn1, TMO_POL) -> E_TMOUT ... (F-S)
 *	26:	twai_flg(FLG2, 0x0fU, TWF_ANDW, &flgptn2, TMO_FEVR)	... (F-S)
 *	== TASK1（続き）==
 *	27:	set_flg(FLG2, 0x05U)
 *	== TASK2（続き）==
 *	28:	assert(flgptn2 == 0xffU)
 *	29:	set_tim((6LLU << 32) + 100U)							... (G-S)
 *		get_tim(&systim)
 *		assert((6LLU << 32) + 100U <= systim \
 *				&& systim <= (6LLU << 32) + 100U + TEST_TIME_PROC)
 *	30:	set_tim((8LLU << 32) + 1000U)							... (G-S)
 *		get_tim(&systim)
 *		assert((8LLU << 32) + 1000U <= systim \
 *				&& systim <= (8LLU << 32) + 1000U + TEST_TIME_PROC)
 *	31:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_calsvc.h"

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task1(intptr_t exinf)
{
	ER_UINT	ercd;
	PRI		intpri;
	T_RFLG	pk_rflg;
	uint_t	load1;
	uint_t	load2;
	FLGPTN	flgptn1;
	FLGPTN	flgptn2;
	SYSTIM	systim;

	test_start(__FILE__);

	check_point(1);
	check_assert(sns_loc() == false);

	ercd = loc_cpu();
	check_ercd(ercd, E_OK);

	check_assert(sns_loc() == true);

	ercd = unl_cpu();
	check_ercd(ercd, E_OK);

	check_assert(sns_loc() == false);

	check_point(2);
	ercd = get_ipm(&intpri);
	check_ercd(ercd, E_OK);

	check_assert(intpri == TIPM_ENAALL);

	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	ercd = get_ipm(&intpri);
	check_ercd(ercd, E_OK);

	check_assert(intpri == TMAX_INTPRI);

	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	ercd = get_ipm(&intpri);
	check_ercd(ercd, E_OK);

	check_assert(intpri == TIPM_ENAALL);

	check_point(3);
	ercd = ref_flg(FLG1, &pk_rflg);
	check_ercd(ercd, E_OK);

	check_assert(pk_rflg.flgptn == 0U);

	ercd = set_flg(FLG1, 0x55U);
	check_ercd(ercd, E_OK);

	ercd = ref_flg(FLG1, &pk_rflg);
	check_ercd(ercd, E_OK);

	check_assert(pk_rflg.flgptn == 0x55U);

	ercd = set_flg(FLG2, 0xaaU);
	check_ercd(ercd, E_OK);

	ercd = ref_flg(FLG2, &pk_rflg);
	check_ercd(ercd, E_OK);

	check_assert(pk_rflg.flgptn == 0xaaU);

	check_point(4);
	ercd = mget_lod(DOM1, HIGH_PRIORITY, &load1);
	check_ercd(ercd, E_OK);

	check_assert(load1 == 1);

	ercd = mget_lod(TDOM_KERNEL, MID_PRIORITY, &load2);
	check_ercd(ercd, E_OK);

	check_assert(load2 == 1);

	check_point(5);
	ercd = wai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1);
	check_ercd(ercd, E_OK);

	check_assert(flgptn1 == 0x55U);

	check_point(6);
	ercd = wai_flg(FLG2, 0xf0U, TWF_ANDW, &flgptn2);
	check_ercd(ercd, E_OK);

	check_point(8);
	check_assert(flgptn2 == 0xfaU);

	check_point(9);
	ercd = twai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1, TMO_POL);
	check_ercd(ercd, E_OK);

	check_assert(flgptn1 == 0x55U);

	check_point(10);
	ercd = twai_flg(FLG1, 0x0aU, TWF_ORW, &flgptn1, TMO_POL);
	check_ercd(ercd, E_TMOUT);

	check_point(11);
	ercd = twai_flg(FLG2, 0x0fU, TWF_ANDW, &flgptn2, TMO_FEVR);
	check_ercd(ercd, E_OK);

	check_point(13);
	check_assert(flgptn2 == 0xffU);

	ercd = clr_flg(FLG1, 0x0U);
	check_ercd(ercd, E_OK);

	ercd = clr_flg(FLG2, 0x0U);
	check_ercd(ercd, E_OK);

	check_point(14);
	ercd = set_tim((2LLU << 32) + 100U);
	check_ercd(ercd, E_OK);

	ercd = get_tim(&systim);
	check_ercd(ercd, E_OK);

	check_assert((2LLU << 32) + 100U <= systim && systim <= (2LLU << 32) + 100U + TEST_TIME_PROC);

	check_point(15);
	ercd = set_tim((4LLU << 32) + 1000U);
	check_ercd(ercd, E_OK);

	ercd = get_tim(&systim);
	check_ercd(ercd, E_OK);

	check_assert((4LLU << 32) + 1000U <= systim && systim <= (4LLU << 32) + 1000U + TEST_TIME_PROC);

	ercd = chg_pri(TSK_SELF, LOW_PRIORITY);
	check_ercd(ercd, E_OK);

	check_point(22);
	ercd = set_flg(FLG2, 0x50U);
	check_ercd(ercd, E_OK);

	check_point(27);
	ercd = set_flg(FLG2, 0x05U);
	check_ercd(ercd, E_OK);

	check_point(0);
}

void
task2(intptr_t exinf)
{
	ER_UINT	ercd;
	PRI		intpri;
	T_RFLG	pk_rflg;
	uint_t	load1;
	uint_t	load2;
	FLGPTN	flgptn1;
	FLGPTN	flgptn2;
	SYSTIM	systim;

	check_point(7);
	ercd = set_flg(FLG2, 0x50U);
	check_ercd(ercd, E_OK);

	check_point(12);
	ercd = set_flg(FLG2, 0x05U);
	check_ercd(ercd, E_OK);

	check_point(16);
	check_assert(sns_loc() == false);

	ercd = loc_cpu();
	check_ercd(ercd, E_OK);

	check_assert(sns_loc() == true);

	ercd = unl_cpu();
	check_ercd(ercd, E_OK);

	check_assert(sns_loc() == false);

	check_point(17);
	ercd = get_ipm(&intpri);
	check_ercd(ercd, E_OK);

	check_assert(intpri == TIPM_ENAALL);

	ercd = chg_ipm(TMAX_INTPRI);
	check_ercd(ercd, E_OK);

	ercd = get_ipm(&intpri);
	check_ercd(ercd, E_OK);

	check_assert(intpri == TMAX_INTPRI);

	ercd = chg_ipm(TIPM_ENAALL);
	check_ercd(ercd, E_OK);

	ercd = get_ipm(&intpri);
	check_ercd(ercd, E_OK);

	check_assert(intpri == TIPM_ENAALL);

	check_point(18);
	ercd = ref_flg(FLG1, &pk_rflg);
	check_ercd(ercd, E_OK);

	check_assert(pk_rflg.flgptn == 0U);

	ercd = set_flg(FLG1, 0x55U);
	check_ercd(ercd, E_OK);

	ercd = ref_flg(FLG1, &pk_rflg);
	check_ercd(ercd, E_OK);

	check_assert(pk_rflg.flgptn == 0x55U);

	ercd = set_flg(FLG2, 0xaaU);
	check_ercd(ercd, E_OK);

	ercd = ref_flg(FLG2, &pk_rflg);
	check_ercd(ercd, E_OK);

	check_assert(pk_rflg.flgptn == 0xaaU);

	check_point(19);
	ercd = mget_lod(DOM1, LOW_PRIORITY, &load1);
	check_ercd(ercd, E_OK);

	check_assert(load1 == 1);

	ercd = mget_lod(TDOM_KERNEL, MID_PRIORITY, &load2);
	check_ercd(ercd, E_OK);

	check_assert(load2 == 1);

	check_point(20);
	ercd = wai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1);
	check_ercd(ercd, E_OK);

	check_assert(flgptn1 == 0x55U);

	check_point(21);
	ercd = wai_flg(FLG2, 0xf0U, TWF_ANDW, &flgptn2);
	check_ercd(ercd, E_OK);

	check_point(23);
	check_assert(flgptn2 == 0xfaU);

	check_point(24);
	ercd = twai_flg(FLG1, 0x0fU, TWF_ORW, &flgptn1, TMO_POL);
	check_ercd(ercd, E_OK);

	check_assert(flgptn1 == 0x55U);

	check_point(25);
	ercd = twai_flg(FLG1, 0x0aU, TWF_ORW, &flgptn1, TMO_POL);
	check_ercd(ercd, E_TMOUT);

	check_point(26);
	ercd = twai_flg(FLG2, 0x0fU, TWF_ANDW, &flgptn2, TMO_FEVR);
	check_ercd(ercd, E_OK);

	check_point(28);
	check_assert(flgptn2 == 0xffU);

	check_point(29);
	ercd = set_tim((6LLU << 32) + 100U);
	check_ercd(ercd, E_OK);

	ercd = get_tim(&systim);
	check_ercd(ercd, E_OK);

	check_assert((6LLU << 32) + 100U <= systim && systim <= (6LLU << 32) + 100U + TEST_TIME_PROC);

	check_point(30);
	ercd = set_tim((8LLU << 32) + 1000U);
	check_ercd(ercd, E_OK);

	ercd = get_tim(&systim);
	check_ercd(ercd, E_OK);

	check_assert((8LLU << 32) + 1000U <= systim && systim <= (8LLU << 32) + 1000U + TEST_TIME_PROC);

	check_finish(31);
	check_point(0);
}
