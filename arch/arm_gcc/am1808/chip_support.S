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
 */

/*
 *		カーネルの割込みコントローラ依存部のアセンブリ言語部（AINTC用）
 */

#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#define UINT_C(val)			(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)		(val)		/* ulong_t型の定数を作るマクロ */
#define CAST(type, val)		(val)		/* 型キャストを行うマクロ */
#include "kernel_impl.h"
#include "core_asm.inc"

/*
 *  AINTC Registers
 */
#define SICR   0xFFFEE024
#define CMR    0xFFFEE400
#define HIPIR2 0xFFFEE904
#define HINLR2 0xFFFEF104

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
     *  割込み発生前の割込み優先度マスクをスタックへ保存
     */
    ldr     r0, =HINLR2
    ldr     r0, [r0]
    str     r0, [sp]

	/*
	 *  割込み要因を取得する．
	 */
    ldr     r2, =HIPIR2
    ldr     r4, [r2] /* r4 <- 割込み要因 */

    /*
	 *  割込み要因の割込み優先度マスクをセット
     */
    ldr     r1, =CMR
    ldrb    r0, [r1, r4] /* r0 <- 割込み要因の優先度 */
    orr     r0, #0x80000000
    ldr     r1, =HINLR2
    str     r0, [r1]
    str     r2, [r2] /* Release frozen registers */

    /*
     *  割込みクリア
     */
    ldr     r1, =SICR
    str     r4, [r1]

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
     *  割込み優先度マスクを元に戻す
     */
    ldr     r1, [sp]                    /* 元の割込み優先度マスクを取得 */
    orr     r1, #0xF0000000
    ldr     r0, =HINLR2
    str     r1, [r0]                    /* 割込み優先度マスクを復帰 */
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
    ldr     r0, =HINLR2
    ldr     r0, [r0]              
    add     r0, r0, #CHN_TO_PRI_OFFSET /* r0(ipm) <- r0(chn) */
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
    ldr     r0, =HINLR2
    ldr     r1, [r0]
    str     r1, [sp]
	bx		lr

/*
 *  CPU例外ハンドラ呼出し後の割込みコントローラ操作
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(irc_end_exc)
ALABEL(irc_end_exc)
    /*
     *  割込み優先度マスクを元に戻す
     */
    ldr     r1, [sp]                    /* 元の割込み優先度マスクを取得 */
    orr     r1, #0xF0000000
    ldr     r0, =HINLR2
    str     r1, [r0]                    /* 割込み優先度マスクを復帰 */
	bx		lr

