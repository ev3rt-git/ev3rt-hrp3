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
 * 
 *  $Id: chip_timer.c 652 2019-01-14 12:57:17Z ertl-hiro $
 */

/*
 *		タイマドライバ（RZ/A1 OSタイマ用）
 *
 *  RZ/A1は2チャンネルのOSタイマを持つが，その内の1つを用いて高分解能
 *  タイマを，もう1つを用いてタイムウィンドウタイマまたはオーバランタ
 *  イマを実現する．タイムウィンドウタイマとオーバランタイマを同時に使
 *  用することはできない．
 */

#include "kernel_impl.h"
#include "domain.h"
#include "time_event.h"
#ifdef TOPPERS_SUPPORT_OVRHDR
#include "overrun.h"
#endif /* TOPPERS_SUPPORT_OVRHDR */
#include "target_timer.h"
#include <sil.h>
#include "rza1.h"

/*
 *  タイマの起動処理
 */
void
target_hrt_initialize(intptr_t exinf)
{
	/*
	 *  OSタイマをフリーランニングコンペアモードに設定する．
	 */
	sil_wrb_mem(OSTM_CTL(OSTM0_BASE), OSTM_CTL_FRCMP);

	/*
	 *  OSタイマの設定値を最大値にしておく．
	 */
	sil_wrw_mem(OSTM_CMP(OSTM0_BASE), 0xffffffff);

	/*
	 *  OSタイマを動作開始する．
	 */
	sil_wrb_mem(OSTM_TS(OSTM0_BASE), OSTM_TS_START);

	/*
	 *  タイマ割込み要求をクリアする．
	 */
	clear_int(INTNO_OSTM0);
}

/*
 *  タイマの停止処理
 */
void
target_hrt_terminate(intptr_t exinf)
{
	/*
	 *  OSタイマを停止する．
	 */
	sil_wrb_mem(OSTM_TT(OSTM0_BASE), OSTM_TT_STOP);

	/*
	 *  タイマ割込み要求をクリアする．
	 */
	clear_int(INTNO_OSTM0);
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
 *  タイムウィンドウタイマ／オーバランタイマドライバ
 */

/*
 *  タイムウィンドウタイマの／オーバランタイマ初期化処理
 */
void
target_totimer_initialize(intptr_t exinf)
{
	/*
	 *  OSタイマをインターバルタイマモードに設定する．
	 */
	sil_wrb_mem(OSTM_CTL(OSTM1_BASE), OSTM_CTL_INTERVAL);

	/*
	 *  タイムウィンドウタイマ割込み要求をクリアする．
	 */
	clear_int(INTNO_OSTM1);
}

/*
 *  タイムウィンドウタイマ／オーバランタイマの停止処理
 */
void
target_totimer_terminate(intptr_t exinf)
{
	/*
	 *  OSタイマを停止する．
	 */
	sil_wrb_mem(OSTM_TT(OSTM1_BASE), OSTM_TT_STOP);

	/*
	 *  タイムウィンドウタイマ割込み要求をクリアする．
	 */
	clear_int(INTNO_OSTM1);
}

/*
 *  タイムウィンドウタイマ／オーバランタイマ割込みハンドラ
 *
 *  このルーチンに来るまでに，target_twdtimer_stopまたは
 *  target_ovrtimer_stopが呼ばれているため，OSタイマを停止する必要はな
 *  い．
 */
void
target_totimer_handler(void)
{
	if (system_cyctim != 0U) {
		twd_switch();				/* タイムウィンドウ切換え処理 */
	}
#ifdef TOPPERS_SUPPORT_OVRHDR
	if (ovrinib.ovrhdr != NULL) {
		call_ovrhdr();				/* オーバランハンドラの起動処理 */
	}
#endif /* TOPPERS_SUPPORT_OVRHDR */
}
