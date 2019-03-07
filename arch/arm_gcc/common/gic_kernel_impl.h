/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2006-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: gic_kernel_impl.h 574 2018-11-27 08:39:05Z ertl-hiro $
 */

/*
 *		カーネルの割込みコントローラ依存部（GIC用）
 *
 *  このヘッダファイルは，target_kernel_impl.h（または，そこからインク
 *  ルードされるファイル）のみからインクルードされる．他のファイルから
 *  直接インクルードしてはならない．
 */

#ifndef TOPPERS_GIC_KERNEL_IMPL_H
#define TOPPERS_GIC_KERNEL_IMPL_H

#include <sil.h>
#include "arm.h"

/*
 *  割込み番号の数，最小値と最大値
 */
#define TNUM_INTNO		GIC_TNUM_INTNO
#define TMIN_INTNO		UINT_C(0)
#define TMAX_INTNO		(GIC_TNUM_INTNO - 1)

/*
 *  割込みハンドラ番号の数
 */
#define TNUM_INHNO		GIC_TNUM_INTNO

/*
 *  割込み番号の定義
 */
#define GIC_INTNO_SGI0		UINT_C(0)
#define GIC_INTNO_PPI0		UINT_C(16)
#define GIC_INTNO_SPI0		UINT_C(32)

/*
 *  割込み優先度の操作
 *
 *  割込み優先度の内部表現は，uint_t型で表し，0が最高優先度で，値が大き
 *  いほど優先度が下がるものとする．GICのレジスタ構成と整合させるために，
 *  優先度の段数が256段階の時にあわせて表す．
 */
#define GIC_PRI_LEVEL		(TMAX_INTPRI - TMIN_INTPRI + 2)

#if GIC_PRI_LEVEL == 16
#define GIC_PRI_SHIFT		4
#define GIC_PRI_MASK		UINT_C(0x0f)
#elif GIC_PRI_LEVEL == 32
#define GIC_PRI_SHIFT		3
#define GIC_PRI_MASK		UINT_C(0x1f)
#elif GIC_PRI_LEVEL == 64
#define GIC_PRI_SHIFT		2
#define GIC_PRI_MASK		UINT_C(0x3f)
#elif GIC_PRI_LEVEL == 128
#define GIC_PRI_SHIFT		1
#define GIC_PRI_MASK		UINT_C(0x7f)
#elif GIC_PRI_LEVEL == 256
#define GIC_PRI_SHIFT		0
#define GIC_PRI_MASK		UINT_C(0xff)
#else
#error Invalid number of priority levels for GIC.
#endif /* GIC_PRI_LEVEL == 16 */

/* 外部表現への変換 */
#define EXT_IPM(pri) \
			(((PRI)((pri) >> GIC_PRI_SHIFT)) - (GIC_PRI_LEVEL - 1))

/* 内部表現への変換 */
#define INT_IPM(ipm) \
			(((uint_t)((ipm) + (GIC_PRI_LEVEL - 1))) << GIC_PRI_SHIFT)

/*
 *  GICレジスタのアドレスを定義するためのマクロ
 *
 *  GICレジスタのアドレスを，アセンブリ言語からも参照できるようにするた
 *  めのマクロ．
 */
#ifndef GIC_REG
#define GIC_REG(base, offset)	((uint32_t *)((base) + (offset ## U)))
#endif /* GIC_REG */

/*
 *  CPUインタフェース関連の定義
 */
#define GICC_CTLR		GIC_REG(GICC_BASE, 0x00)
#define GICC_PMR		GIC_REG(GICC_BASE, 0x04)
#define GICC_BPR		GIC_REG(GICC_BASE, 0x08)
#define GICC_IAR		GIC_REG(GICC_BASE, 0x0C)
#define GICC_EOIR		GIC_REG(GICC_BASE, 0x10)
#define GICC_RPR		GIC_REG(GICC_BASE, 0x14)
#define GICC_HPPIR		GIC_REG(GICC_BASE, 0x18)

/*
 *  CPUインタフェース制御レジスタ（GICC_CTLR）の設定値（GICv1でセキュリ
 *  ティ拡張がない場合）
 */
#define GICC_CTLR_DISABLE		UINT_C(0x00)
#define GICC_CTLR_ENABLE		UINT_C(0x01)

/*
 *  CPUインタフェース制御レジスタ（GICC_CTLR）の設定値（GICv2でセキュリ
 *  ティ拡張がない場合か，拡張がある場合のセキュアモード）
 */
#ifdef TOPPERS_SAFEG_SECURE
#define GICC_CTLR_ENABLEGRP0	UINT_C(0x01)
#define GICC_CTLR_ENABLEGRP1	UINT_C(0x02)
#define GICC_CTLR_FIQEN			UINT_C(0x08)
#define GICC_CTLR_CBPR			UINT_C(0x10)
#endif /* TOPPERS_SAFEG_SECURE */

/*
 *  ディストリビュータ関連の定義
 */
#define GICD_CTLR			GIC_REG(GICD_BASE, 0x000)
#define GICD_TYPER			GIC_REG(GICD_BASE, 0x004)
#define GICD_IIDR			GIC_REG(GICD_BASE, 0x008)
#define GICD_IGROUPR(n)		GIC_REG(GICD_BASE, 0x080 + (n) * 4)
#define GICD_ISENABLER(n)	GIC_REG(GICD_BASE, 0x100 + (n) * 4)
#define GICD_ICENABLER(n)	GIC_REG(GICD_BASE, 0x180 + (n) * 4)
#define GICD_ISPENDR(n)		GIC_REG(GICD_BASE, 0x200 + (n) * 4)
#define GICD_ICPENDR(n)		GIC_REG(GICD_BASE, 0x280 + (n) * 4)
#define GICD_ISACTIVER(n)	GIC_REG(GICD_BASE, 0x300 + (n) * 4)
#define GICD_ICACTIVER(n)	GIC_REG(GICD_BASE, 0x380 + (n) * 4)
#define GICD_IPRIORITYR(n)	GIC_REG(GICD_BASE, 0x400 + (n) * 4)
#define GICD_ITARGETSR(n)	GIC_REG(GICD_BASE, 0x800 + (n) * 4)
#define GICD_ICFGR(n)		GIC_REG(GICD_BASE, 0xc00 + (n) * 4)
#define GICD_NSCAR(n)		GIC_REG(GICD_BASE, 0xe00 + (n) * 4)
#define GICD_SGIR			GIC_REG(GICD_BASE, 0xf00)
#define GICD_CPENDSGIR(n)	GIC_REG(GICD_BASE, 0xf10 + (n) * 4)
#define GICD_SPENDSGIR(n)	GIC_REG(GICD_BASE, 0xf20 + (n) * 4)

/*
 *  ディストリビュータ制御レジスタ（GICD_CTLR）の設定値
 */
#define GICD_CTLR_DISABLE	UINT_C(0x00)
#define GICD_CTLR_ENABLE	UINT_C(0x01)

/*
 *  割込みコンフィギュレーションレジスタ（GICD_ICFGRn）の設定値
 *
 *  第1ビットは，ARM11 MPCoreおよびGICの早い時期の実装では割込みの通知
 *  先プロセッサを設定するために使用されている．
 */
#define GICD_ICFGRn_LEVEL	UINT_C(0x00)
#define GICD_ICFGRn_EDGE	UINT_C(0x02)
#ifdef GIC_ARM11MPCORE
#define GICD_ICFGRn_N_N		UINT_C(0x00)
#define GICD_ICFGRn_1_N		UINT_C(0x01)
#endif /* GIC_ARM11MPCORE */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  CPUインタフェースの操作
 */

/*
 *  割込み優先度マスクを設定（priは内部表現）
 */ 
Inline void
gicc_set_priority(uint_t pri)
{
	sil_swrw_mem(GICC_PMR, pri);
}

/*
 *  割込み優先度マスクを取得（内部表現で返す）
 */ 
Inline uint_t
gicc_get_priority(void)
{
	return(sil_rew_mem(GICC_PMR));
}

/*
 *  CPUインタフェースの初期化
 */
extern void gicc_initialize(void);

/*
 *  CPUインタフェースの終了
 */
extern void gicc_terminate(void);

/*
 *  ディストリビュータの操作
 */

/*
 *  割込み禁止（割込みイネーブルのクリア）
 */
Inline void
gicd_disable_int(INTNO intno)
{
	sil_swrw_mem(GICD_ICENABLER(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込み許可（割込みイネーブルのセット）
 */
Inline void
gicd_enable_int(INTNO intno)
{
	sil_swrw_mem(GICD_ISENABLER(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込みペンディングのクリア
 */
Inline void
gicd_clear_pending(INTNO intno)
{
	sil_swrw_mem(GICD_ICPENDR(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込みペンディングのセット
 */
Inline void
gicd_set_pending(INTNO intno)
{
	sil_swrw_mem(GICD_ISPENDR(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込みペンディングのチェック
 */
Inline bool_t
gicd_probe_pending(INTNO intno)
{
	return((sil_rew_mem(GICD_ISPENDR(intno / 32)) & (1U << (intno % 32)))
																	!= 0U);
}

/*
 *  ソフトウェア生成割込み（SGI）の生成
 */
Inline void
gicd_raise_sgi(INTNO intno)
{
	sil_swrw_mem(GICD_SGIR, (0x02000000 | intno));
}

/*
 *  割込みのコンフィグレーション
 */
Inline void
gicd_config(INTNO intno, uint_t config)
{
	uint_t		shift = (intno % 16) * 2;
	uint32_t	reg;

	if (intno >= GIC_INTNO_PPI0) {
		reg = sil_rew_mem(GICD_ICFGR(intno / 16));
		reg &= ~(0x03U << shift);
		reg |= (config << shift);
		sil_wrw_mem(GICD_ICFGR(intno / 16), reg);
	}
}

/*
 *  割込みグループの設定（セキュリティ拡張）
 */
Inline void
gicd_config_group(INTNO intno, uint_t group)
{
	uint_t		shift = intno % 32;
	uint32_t	reg;

	reg = sil_rew_mem(GICD_IGROUPR(intno / 32));
	reg &= ~(0x01U << shift);
	reg |= (group << shift);
	sil_wrw_mem(GICD_IGROUPR(intno / 32), reg);
}

/*
 *  割込み要求ラインに対する割込み優先度の設定（priは内部表現）
 */
Inline void
gicd_set_priority(INTNO intno, uint_t pri)
{
	uint_t		shift = (intno % 4) * 8;
	uint32_t	reg;

	reg = sil_rew_mem(GICD_IPRIORITYR(intno / 4));
	reg &= ~(0xffU << shift);
	reg |= (pri << shift);
	sil_wrw_mem(GICD_IPRIORITYR(intno / 4), reg);
}

/*
 *  割込みターゲットプロセッサの設定
 *
 *  prcsは，ターゲットとするプロセッサを表すビットのビット毎論理和で指
 *  定する．
 *		プロセッサ0 : 0x01
 *		プロセッサ1 : 0x02
 *		プロセッサ2 : 0x04
 *		プロセッサ3 : 0x08
 */
Inline void
gicd_set_target(INTNO intno, uint_t prcs)
{
	uint_t		shift = (intno % 4) * 8;
	uint32_t	reg;

	reg = sil_rew_mem(GICD_ITARGETSR(intno / 4));
	reg &= ~(0xffU << shift);
	reg |= (prcs << shift);
	sil_wrw_mem(GICD_ITARGETSR(intno / 4), reg);
}

/*
 *  ディストリビュータの初期化
 */
extern void gicd_initialize(void);

/*
 *  ディストリビュータの終了
 */
extern void gicd_terminate(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  割込み番号の範囲の判定
 */
#define VALID_INTNO(intno)	(TMIN_INTNO <= (intno) && (intno) <= TMAX_INTNO)

/*
 *  割込み要求ラインのための標準的な初期化情報を生成する
 */
#define USE_INTINIB_TABLE

/*
 *  割込み要求ライン設定テーブルを生成する
 */
#define USE_INTCFG_TABLE

/*
 *  コア依存部
 */
#include "core_kernel_impl.h"

/*
 *  ターゲット非依存部に提供する関数
 */
#ifndef TOPPERS_MACRO_ONLY

/*
 *  割込み属性の設定のチェック
 */
Inline bool_t
check_intno_cfg(INTNO intno)
{
	return(intcfg_table[intno] != 0U);
}

/*
 *  割込み優先度マスクの設定
 */
Inline void
t_set_ipm(PRI intpri)
{
	gicc_set_priority(INT_IPM(intpri));
}

/*
 *  割込み優先度マスクの参照
 */
Inline PRI
t_get_ipm(void)
{
	return(EXT_IPM(gicc_get_priority()));
}

/*
 *  割込み要求禁止フラグが操作できる割込み番号の範囲の判定
 */
#ifdef GIC_SUPPORT_DISABLE_SGI
#define VALID_INTNO_DISINT(intno)	VALID_INTNO(intno)
#else /* GIC_SUPPORT_DISABLE_SGI */
#define VALID_INTNO_DISINT(intno) \
				(GIC_INTNO_PPI0 <= (intno) && (intno) <= TMAX_INTNO)
#endif /* GIC_SUPPORT_DISABLE_SGI */

/*
 *  割込み要求禁止フラグのセット
 */
Inline void
disable_int(INTNO intno)
{
	gicd_disable_int(intno);
}

/* 
 *  割込み要求禁止フラグのクリア
 */
Inline void
enable_int(INTNO intno)
{
	gicd_enable_int(intno);
}

/*
 *  割込み要求がクリアできる割込み番号の範囲の判定
 */
#define VALID_INTNO_CLRINT(intno) \
				(GIC_INTNO_PPI0 <= (intno) && (intno) <= TMAX_INTNO)

/*
 *  割込み要求がクリアできる状態か？
 */
Inline bool_t
check_intno_clear(INTNO intno)
{
	return(true);
}

/*
 *  割込み要求のクリア
 */
Inline void
clear_int(INTNO intno)
{
	gicd_clear_pending(intno);
}

/*
 *  割込みが要求できる状態か？
 */
Inline bool_t
check_intno_raise(INTNO intno)
{
	return(true);
}

/*
 *  割込みの要求
 */
Inline void
raise_int(INTNO intno)
{
	if (intno < GIC_INTNO_PPI0) {
		gicd_raise_sgi(intno);
	}
	else {
		gicd_set_pending(intno);
	}
}

/*
 *  割込み要求のチェック
 */
Inline bool_t
probe_int(INTNO intno)
{
	return(gicd_probe_pending(intno));
}

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_GIC_KERNEL_IMPL_H */
