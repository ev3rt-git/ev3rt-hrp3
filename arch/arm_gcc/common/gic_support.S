/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
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
 *  $Id: gic_support.S 255 2018-02-11 18:25:21Z ertl-hiro $
 */

/*
 *		カーネルの割込みコントローラ依存部のアセンブリ言語部（GIC用）
 */

#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#define UINT_C(val)			(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)		(val)		/* ulong_t型の定数を作るマクロ */
#define CAST(type, val)		(val)		/* 型キャストを行うマクロ */
#define GIC_REG(base, offset)	((base) + (offset))
#include "kernel_impl.h"
#include "core_asm.inc"

#if __TARGET_ARCH_ARM < 6
#error gic_suport.S supports ARMv6 or later.
#endif /* __TARGET_ARCH_ARM < 6 */

/*
 *  割込みハンドラ呼出し前の割込みコントローラ操作
 *
 *  r4に割込み番号を返す．irc_end_intで用いる情報（割込み発生前の割込み
 *  優先度マスク）を，スタックの先頭に保存する．
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(irc_begin_int)
ALABEL(irc_begin_int)
	/*
	 *  割込み要因を取得する．
	 */
#ifdef GIC_PL390_ERRATA			/* GIC 390 Errata 801120への対策 */
	ldr		r1, =GICC_HPPIR
	ldr		r0, [r1]	
#endif /* GIC_PL390_ERRATA */
	ldr		r1, =GICC_IAR
	ldr		r3, [r1]
	lsl		r4, r3, #22			/* 下位10ビットを取り出す */
	lsr		r4, r4, #22

#ifdef GIC_PL390_ERRATA			/* GIC 390 Errata 733075への対策 */
	movw	r0, #1023
	cmp		r4, r0
	beq		irc_begin_int_errata_1
	movw	r0, #1022
	cmp		r4, r0
	beq		irc_begin_int_errata_1
	cmp		r3, #0
	bne		irc_begin_int_errata_2
	ldr		r1, =GICD_ISACTIVER(0)		/* 割込み要求があるかチェック */
	ldr		r0, [r1]
	tst		r0, #0x01
	movweq	r4, #1024					/* 無効な割込みとみなす */
ALABEL(irc_begin_int_errata_1)
	ldr		r1, =GICD_IPRIORITYR(0)		/* 割込み優先度レジスタ0に書き込み */
	ldr		r0, [r1]
	str		r0, [r1]
	asm_data_sync_barrier r0
ALABEL(irc_begin_int_errata_2)
#endif /* GIC_PL390_ERRATA */

	/*
	 *  割込み要因の割込み優先度を求め，割込み優先度マスクに設定する．
	 */
	ldr		r1, =GICC_RPR		/* 受け付けた割込みの割込み優先度を取得 */
	ldr		r0, [r1]
	ldr		r1, =GICC_PMR		/* 割込み発生前の割込み優先度を取得 */
	ldr		r2, [r1]
	str		r0, [r1]			/* 新しい割込み優先度マスクをセットする */
	asm_data_sync_barrier r0	/* 割込み優先度マスクがセットされるのを待つ */
	str		r2, [sp]			/* irc_end_intで用いる情報を保存 */

	/*
	 *  EOIを発行する．
	 */
	ldr		r1, =GICC_EOIR		/* EOIレジスタへの書込み */
	str		r3, [r1]

	/*
	 *  r4に割込み番号を入れた状態でリターンする．
	 */
	bx		lr

/*
 *  割込みハンドラ呼出し後の割込みコントローラ操作
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(irc_end_int)
ALABEL(irc_end_int)
	/*
	 *  割込み優先度マスクを元に戻す．
	 */
	ldr		r2, [sp]			/* irc_begin_intで保存した情報を復帰 */
	ldr		r1, =GICC_PMR		/* 割込み優先度マスクを元に戻す */
	str		r2, [r1]
	bx		lr

/*
 *  CPU例外発生前の割込み優先度の取得
 *
 *  CPU例外の発生で割込み優先度が変わることはないため，現在の割込み優先
 *  度を返す．
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(irc_get_intpri)
ALABEL(irc_get_intpri)
	/*
	 *  割込み優先度マスクを外部表現に変換して返す．
	 */
	ldr		r1, =GICC_PMR
	ldr		r0, [r1]
	asr		r0, r0, #GIC_PRI_SHIFT
	sub		r0, r0, #GIC_PRI_MASK
	bx		lr

/*
 *  CPU例外ハンドラ呼出し前の割込みコントローラ操作
 *
 *  irc_end_excで用いる情報（CPU例外発生前の割込み優先度マスク）を，スタッ
 *	クの先頭に保存する．
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(irc_begin_exc)
ALABEL(irc_begin_exc)
	/*
	 *  割込み優先度マスクを保存する．
	 */
	ldr		r1, =GICC_PMR		/* 現在の割込み優先度を取得 */
	ldr		r2, [r1]
	str		r2, [sp]			/* irc_end_excで用いる情報を保存 */
	bx		lr

/*
 *  CPU例外ハンドラ呼出し後の割込みコントローラ操作
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(irc_end_exc)
ALABEL(irc_end_exc)
	/*
	 *  割込み優先度マスクを元に戻す．
	 */
	ldr		r2, [sp]			/* irc_begin_excで保存した情報を復帰 */
	ldr		r1, =GICC_PMR		/* 割込み優先度マスクを元に戻す */
	str		r2, [r1]
	bx		lr
