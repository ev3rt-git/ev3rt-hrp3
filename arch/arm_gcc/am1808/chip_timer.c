/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2006-2019 by Embedded and Real-Time Systems Laboratory
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
 *		タイマドライバ（AM1808用）
 *
 *  TIMER64P0.TIM12: 高分解能タイマ（現在値）
 *  TIMER64P0.TIM34: 高分解能タイマ（差分）
 *  TIMER64P1.TIM12: タイムウィンドウタイマ
 *  高分解能タイマ: TIMER64P0.TIM12（現在値），TIMER64P0.TIM34（差分）
 *  タイムウィンドウタイマ: TIMER64P1.TIM12
 */

#include "kernel_impl.h"
#include "domain.h"
#include "time_event.h"
#include "target_timer.h"
#include <sil.h>

/*
 *  タイマの起動処理
 */
void
target_hrt_initialize(intptr_t exinf)
{
    /*
     * タイマ停止し，クリアする
     */
    TIMERP0.TGCR  = 0x0;
    TIMERP0.TIM12 = 0x0;

	/*
	 *  タイマ周期を設定
	 */
    TIMERP0.PRD12 = HRTCNT_TO_CMWCNT(TCYC_HRTCNT) - 1;
    TIMERP0.REL12 = TIMERP0.PRD12;

	/*
	 *  タイマ割込み要求をクリアする
	 */
	clear_int(INTNO_TIMER);

    /*
     * タイマ再開
     */
    TIMERP0.INTCTLSTAT = 0x30000;
    TIMERP0.EMUMGT = 0x1;
    TIMERP0.TCR  = 0xc0;
    TIMERP0.TGCR = 0x17;
}

/*
 *  タイマの停止処理
 */
void
target_hrt_terminate(intptr_t exinf)
{
    /*
     *  タイマを停止する
     */
    TIMERP0.TGCR  = 0x0;

	/*
	 *  タイマ割込み要求をクリアする
	 */
	clear_int(INTNO_TIMER);
}

/*
 *  タイマ割込みハンドラ
 */
void
target_hrt_handler(void)
{
	/*
	 *  高分解能タイマ割込みを処理する．
	 */
	signal_time();
}

/*
 *  タイムウィンドウタイマドライバ
 */

/*
 *  タイムウィンドウタイマの初期化処理
 */
void
target_twdtimer_initialize(intptr_t exinf)
{
    /*
     *  タイマ停止し，クリアする
     */
    TIMERP1.TGCR  = 0x0;
    TIMERP1.TIM12 = 0x0;
    TIMERP1.PRD12 = 0x0;
    TIMERP1.INTCTLSTAT = 0x3;
    TIMERP1.EMUMGT = 0x1;
    TIMERP1.TCR = 0x0;
    TIMERP1.TGCR = 0x15;

	/*
	 *  タイムウィンドウタイマ割込み要求をクリアする
	 */
	clear_int(INTNO_TWDTIMER);
}

/*
 *  タイムウィンドウタイマの停止処理
 */
void
target_twdtimer_terminate(intptr_t exinf)
{
    /*
     * タイマを停止する
     */
    TIMERP1.TGCR  = 0x0;

	/*
	 *  タイムウィンドウタイマ割込み要求をクリアする．
	 */
	clear_int(INTNO_TWDTIMER);
}

/*
 *  タイムウィンドウタイマ割込みハンドラ
 *
 *  このルーチンに来るまでに，target_twdtimer_stop
 *  が呼ばれているため，タイマを停止する必要はな
 *  い．
 */
void
target_twdtimer_handler(void)
{
	if (system_cyctim != 0U) {
		twd_switch();				/* タイムウィンドウ切換え処理 */
	}
}
