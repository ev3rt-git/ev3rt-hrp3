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
 *		カーネルのチップ依存部（AM1808用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "interrupt.h"
#include "target_serial.h"

extern void* vector_table();

/*
 *  チップ依存の初期化
 */
void
chip_initialize(void)
{
    /*
     *  ベクタテーブルをHigh exception vectors(0xFFFF0000)にコピー
     */
    char *vt  = (char*)0xFFFF0000;
    char *vts = (char*)vector_table;
    for(int i = 0; i < 512; ++i)
        vt[i] = vts[i];
    
    /*
     *  High exception vectorsを使うように設定
     */
    arm_set_high_vectors();

    /*
     *  AINTCの初期化
     */
    AINTC.GER  = 0x1;
    AINTC.HIER = 0x2;
    AINTC.CR   = 0x14;
    t_set_ipm(0); /* Set IPM to 0 */

    /**
     * 低レベル出力シリアルI/Oの初期化
     */
    sio_initialize_low();

	/*
	 *  キャッシュをディスエーブル
	 */
	arm_disable_cache();

	/*
	 *  コア依存の初期化
	 */
	core_initialize();

	/*
	 *  キャッシュをイネーブル
	 */
	arm_enable_cache();
}

/*
 *  チップ依存の終了処理
 */
void
chip_terminate(void)
{
	extern void    software_term_hook(void);
	void (*volatile fp)(void) = software_term_hook;

	/*
	 *  software_term_hookへのポインタを，一旦volatile指定のあるfpに代
	 *  入してから使うのは，0との比較が最適化で削除されないようにするた
	 *  めである．
	 */
	if (fp != 0) {
		(*fp)();
	}

	/*
	 *  コア依存の終了処理
	 */
	core_terminate();
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  HRPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．
 */
static inline void
config_int(INTNO intno, ATR intatr, PRI intpri)
{
	assert(VALID_INTNO(intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込み要求のマスク
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */    
	disable_int(intno);

	/*
	 *  割込み優先度をセット
	 */
    AINTC.CMR[intno] = PRI_TO_CHN(intpri);
    
	/*
	 *  割込みを許可
	 */
	if ((intatr & TA_ENAINT) != 0U){
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

