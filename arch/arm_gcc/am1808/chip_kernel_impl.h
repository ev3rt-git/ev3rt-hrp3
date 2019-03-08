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
 */

/*
 *		kernel_impl.hのチップ依存部（AM1808用）
 *
 *  このヘッダファイルは，target_kernel_impl.h（または，そこからインク
 *  ルードされるファイル）のみからインクルードされる．他のファイルから
 *  直接インクルードしてはならない．
 */

#ifndef TOPPERS_CHIP_KERNEL_IMPL_H
#define TOPPERS_CHIP_KERNEL_IMPL_H

/*
 *  AM1808のハードウェア資源の定義
 */
#include "am1808.h"

/*
 *  割込み番号の数，最小値と最大値
 */
#define TMIN_INTNO      UINT_C(0)
#define TMAX_INTNO      UINT_C(100)
#define TNUM_INTNO      (TMAX_INTNO - TMIN_INTNO + UINT_C(1))

/*
 *  割込みハンドラ番号の数
 */
#define TNUM_INHNO      TNUM_INTNO

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#ifndef DEFAULT_ISTKSZ
#define DEFAULT_ISTKSZ  0x10000U			/* 64KB */
#endif /* DEFAULT_ISTKSZ */

/*
 *  Priority = Channel + CHN_TO_PRI_OFFSET
 */
#define CHN_TO_PRI_OFFSET (-33)

/*
 *  優先度とチャンネル間の変換マクロ
 */
#define PRI_TO_CHN(x) ((x)-CHN_TO_PRI_OFFSET)
#define CHN_TO_PRI(x) ((x)+CHN_TO_PRI_OFFSET)

/*
 *  ARM926EJSにデータ同期バリアと命令同期バリアはない
 */
#define ASM_DATA_SYNC_BARRIER(...)
#define ASM_INST_SYNC_BARRIER(...)
#define DATA_SYNC_BARRIER(...)
#define INST_SYNC_BARRIER(...)

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
 *  MMUの使用に関する設定
 */
#define USE_ARM_MMU

/*
 *  コア依存部
 */
#include "core_kernel_impl.h"

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
    AINTC.HINLR2 = PRI_TO_CHN(intpri) | 0xF0000000;
}

/*
 *  割込み優先度マスクの参照
 */
Inline PRI
t_get_ipm(void)
{
	return(CHN_TO_PRI(AINTC.HINLR2));
}

/*
 *  割込み要求禁止フラグのセット
 */
Inline void
disable_int(INTNO intno)
{
    AINTC.EICR = intno;
}

/* 
 *  割込み要求禁止フラグのクリア
 */
Inline void
enable_int(INTNO intno)
{
    AINTC.EISR = intno;
}

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
    AINTC.SICR = intno;
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
    AINTC.SISR = intno;
}

/*
 *  チップ依存の初期化
 */
extern void chip_initialize(void);

/*
 *  チップ依存の終了処理
 */
extern void chip_terminate(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CHIP_KERNEL_IMPL_H */
