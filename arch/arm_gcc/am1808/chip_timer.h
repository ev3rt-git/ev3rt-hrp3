/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2018 by Embedded and Real-Time Systems Laboratory
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

#ifndef TOPPERS_CHIP_TIMER_H
#define TOPPERS_CHIP_TIMER_H

/*
 *  タイマ割込みハンドラ登録のための定数
 */
#define INHNO_TIMER		T64P0_TINT34			/* 割込みハンドラ番号 */
#define INTNO_TIMER		T64P0_TINT34			/* 割込み番号 */
//#define INTPRI_TIMER	(TMAX_INTPRI - 1)	    /* 割込み優先度 */
#define INTATR_TIMER	TA_NULL				    /* 割込み属性 */

/*
 *  タイムウィンドウタイマ割込みハンドラ登録のための定数
 */
#define INHNO_TWDTIMER		T64P1_TINT12		/* 割込みハンドラ番号 */
#define INTNO_TWDTIMER		T64P1_TINT12		/* 割込み番号 */
#define INTPRI_TWDTIMER		INTPRI_TIMER		/* 割込み優先度 */
#define INTATR_TWDTIMER		TA_NULL				/* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  高分解能タイマの起動処理
 */
extern void	target_hrt_initialize(intptr_t exinf);

/*
 *  高分解能タイマの停止処理
 */
extern void	target_hrt_terminate(intptr_t exinf);

/*
 *  高分解能タイマの現在のカウント値の読出し
 */
Inline HRTCNT
target_hrt_get_current(void)
{
    return((HRTCNT)(CMWCNT_TO_HRTCNT(TIMERP0.TIM12)));
}

/*
 *  高分解能タイマへの割込みタイミングの設定
 *
 *  高分解能タイマを，hrtcntで指定した値カウントアップしたら割込みを発
 *  生させるように設定する．
 */
Inline void
target_hrt_set_event(HRTCNT hrtcnt)
{
    /* Disable and clear */
    TIMERP0.TCR = 0xc0;
    TIMERP0.TIM34 = 0;
    TIMERP0.INTCTLSTAT = 0x30000;

    /* Set target and enable */
    TIMERP0.PRD34 = HRTCNT_TO_CMWCNT(hrtcnt);
    TIMERP0.TCR = 0x4000c0;
}

/*
 *  高分解能タイマ割込みの要求
 */
Inline void
target_hrt_raise_event(void)
{
	raise_int(INTNO_TIMER);
}

/*
 *  高分解能タイマ割込みハンドラ
 */
extern void	target_hrt_handler(void);

/*
 *  タイムウィンドウタイマの初期化処理
 */
extern void target_twdtimer_initialize(intptr_t exinf);

/*
 *  タイムウィンドウタイマの停止処理
 */
extern void target_twdtimer_terminate(intptr_t exinf);

/*
 *  タイムウィンドウタイマの動作開始
 */
Inline void
target_twdtimer_start(PRCTIM twdtim)
{
    if (twdtim > 0) {
        /* Set target and enable */
        TIMERP1.PRD12 = PRCTIM_TO_CMWCNT(twdtim);
        TIMERP1.TCR |= 0x40;
    } else {
        raise_int(INTNO_TWDTIMER);
    }
}

/*
 *  タイムウィンドウタイマの停止
 */
Inline PRCTIM
target_twdtimer_stop(void)
{
    PRCTIM prctim = 0;

    /* Disable */
    TIMERP1.TCR &= ~0x40;

    /* Check interrupt */
    if ((TIMERP1.INTCTLSTAT & 0x2) == 0) {
        /* Read current value */
	    uint32_t current, compare;
        current = TIMERP1.TIM12;
        compare = TIMERP1.PRD12;
        assert(compare >= current);
        prctim = ((PRCTIM)(CMWCNT_TO_PRCTIM(compare - current)));
    } else {
        TIMERP1.INTCTLSTAT = 0x10003;
        clear_int(INTNO_TWDTIMER);
    }

    /* Clear */
    TIMERP1.TIM12 = 0;
    TIMERP1.PRD12 = 0;

    return prctim;
}

/*
 *  タイムウィンドウタイマの現在値の読出し
 */
Inline PRCTIM
target_twdtimer_get_current(void)
{
	uint32_t	current, compare;

    /* Read current value */
    current = TIMERP1.TIM12;
    compare = TIMERP1.PRD12;

    if (current >= compare) {
        return 0;
    }

    return((PRCTIM)(CMWCNT_TO_PRCTIM(compare - current)));
}

/*
 *  タイムウィンドウタイマ割込みハンドラ
 */
extern void	target_twdtimer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CHIP_TIMER_H */
