/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  $Id: chip_kernel_impl.c 543 2018-11-19 15:03:31Z ertl-hiro $
 */

/*
 *		カーネルのチップ依存部（RZ/A1用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "interrupt.h"
#include "pl310.h"

/*
 *  チップ依存の初期化
 */
void
chip_initialize(void)
{
	/*
	 *  MPCore依存の初期化
	 */
	mpcore_initialize();

	/*
	 *  L2キャッシュコントローラ（PL310）の初期化
	 */
	pl310_initialize(0x0U, ~0x0U);
}

/*
 *  チップ依存の終了処理
 */
void
chip_terminate(void)
{
	/*
	 *  MPCore依存の終了処理
	 */
	mpcore_terminate();
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  HRPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．
 */
Inline void
config_int(INTNO intno, ATR intatr, PRI intpri)
{
	assert(VALID_INTNO(intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込みを禁止
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */
	disable_int(intno);

	/*
	 *  割込みをコンフィギュレーション
	 */
#ifdef TOPPERS_SAFEG_SECURE
	gicd_config_group(intno, 1U);
#endif /* TOPPERS_SAFEG_SECURE */

	if (INTNO_IRQ0 <= intno && intno <= INTNO_IRQ7) {
		uint16_t	reg;

		reg = sil_reh_mem(RZA1_ICR1);
		reg &= ~(0x03U << ((intno - INTNO_IRQ0) * 2));
		reg |= ((intatr >> 2) << ((intno - INTNO_IRQ0) * 2));
		sil_wrh_mem(RZA1_ICR1, reg);
	}

	if ((intatr & TA_EDGE) != 0U) {
#ifdef GIC_ARM11MPCORE
		gicd_config(intno, GICD_ICFGRn_EDGE|GICD_ICFGRn_1_N);
#else /* GIC_ARM11MPCORE */
		gicd_config(intno, GICD_ICFGRn_EDGE);
#endif /* GIC_ARM11MPCORE */
		clear_int(intno);
	}
	else {
#ifdef GIC_ARM11MPCORE
		gicd_config(intno, GICD_ICFGRn_LEVEL|GICD_ICFGRn_1_N);
#else /* GIC_ARM11MPCORE */
		gicd_config(intno, GICD_ICFGRn_LEVEL);
#endif /* GIC_ARM11MPCORE */
	}

	/*
	 *  割込み優先度とターゲットプロセッサを設定
	 */
	gicd_set_priority(intno, INT_IPM(intpri));
	gicd_set_target(intno, 1U << arm_prc_index());

	/*
	 * 割込みを許可
	 */
	if ((intatr & TA_ENAINT) != 0U) {
		enable_int(intno);
	}
}

/*
 *  割込み管理機能の初期化
 */
void
initialize_interrupt(void)
{
	uint_t			i;
	const INTINIB	*p_intinib;

	for (i = 0; i < tnum_cfg_intno; i++) {
		p_intinib = &(intinib_table[i]);
		config_int(p_intinib->intno, p_intinib->intatr, p_intinib->intpri);
	}
}
