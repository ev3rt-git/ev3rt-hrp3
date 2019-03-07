/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2008-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: domain.h 610 2018-12-17 08:01:38Z ertl-hiro $
 */

/*
 *		保護ドメイン管理モジュール
 */

#ifndef TOPPERS_DOMAIN_H
#define TOPPERS_DOMAIN_H

#include <queue.h>
#include "time_event.h"

/*
 *  システム周期（kernel_cfg.c）
 *
 *  時間パーティショニングを使用しない場合は0にする．
 */
extern const RELTIM	system_cyctim;

/*
 *  スケジューリング単位管理ブロック
 *
 *  p_predtskは，そのスケジューリング単位の中で優先順位が最も高いタス
 *  クのTCBを指すポインタである．実行できるタスクがない場合はNULLにす
 *  る．
 *
 *  ready_queueは，実行できる状態のタスクを管理するためのキューである．
 *  実行状態のタスクも管理しているため，レディ（実行可能）キューという
 *  名称は正確ではないが，レディキューという名称が定着しているため，こ
 *  の名称で呼ぶことにする．レディキューは，優先度ごとのタスクキューで
 *  構成されている．タスクのTCBは，該当する優先度のキューに登録される．
 *
 *  ready_primapは，優先度ごとのタスクキューにタスクが入っているかどう
 *  かを示すビットマップであり，レディキューのサーチを効率よく行うため
 *  用意している．ビットマップを使うことで，メモリアクセスの回数を減ら
 *  すことができるが，ビット操作命令が充実していないプロセッサで，優先
 *  度の段階数が少ない場合には，ビットマップ操作のオーバーヘッドのため
 *  に，逆に効率が落ちる可能性もある．優先度が16段階であることを仮定し
 *  ているため，uint16_t型としている．
 */
typedef struct scheduing_unit_control_block {
	TCB			*p_predtsk;					/* 優先順位が最も高いタスク */
	QUEUE		ready_queue[TNUM_TPRI];		/* レディキュー */
	uint16_t	ready_primap;	/* レディキューサーチのためのビットマップ */
} SCHEDCB;

/*
 *  現在スケジューリング単位
 *
 *  主に実行すべきスケジューリング単位．タイムウィンドウの実行中は，実
 *  行中のタイムウィンドウを割り当てられたユーザドメインに対応するスケ
 *  ジューリング単位に設定する．
 */
extern SCHEDCB	*p_twdsched;

/*
 *  アイドル時スケジューリング単位
 *
 *  現在スケジューリング単位とカーネルドメインに，実行できるタスクがな
 *  い場合に，実行すべきスケジューリング単位．タイムウィンドウの実行中
 *  は，アイドルドメインに対応するスケジューリング単位に設定する．
 */
extern SCHEDCB	*p_idlesched;

/*
 *  カーネルドメインに対応するスケジューリング単位管理ブロック
 */
extern SCHEDCB	schedcb_kernel;

/*
 *  スケジューリング単位管理ブロックの数（kernel_cfg.c）
 *
 *  タイムウィンドウを割り当てられたユーザドメインの数．
 */
extern const uint_t	tnum_schedcb;

/*
 *  スケジューリング単位管理ブロックのエリア（kernel_cfg.c）
 *
 *  タイムウィンドウを割り当てられたユーザドメインに対応するスケジュー
 *  リング単位管理ブロック．
 */
extern SCHEDCB	schedcb_table[];

/*
 *  アイドルドメインに対応するスケジューリング単位管理ブロック
 */
extern SCHEDCB	schedcb_idle;

/*
 *  タイムウィンドウ初期化ブロック
 */
typedef struct time_window_initialization_block {
	PRCTIM			twdlen;			/* タイムウィンドウ長 */
	const DOMINIB	*p_dominib;		/* 保護ドメイン初期化ブロック */
	intptr_t		exinf;			/* 通知ハンドラの拡張情報 */
	NFYHDR			nfyhdr;			/* 通知ハンドラの起動番地 */
} TWDINIB;

/*
 *  システム動作モード初期化ブロック
 */
typedef struct system_operation_mode_initialization_block SOMINIB;

struct system_operation_mode_initialization_block {
	const TWDINIB	*p_twdinib;		/* タイムウィンドウ初期化ブロック */
	const SOMINIB	*p_nxtsom;		/* 次のシステム動作モード */
};

/*
 *  現在のシステム動作モード
 *
 *  現在のシステム動作モードの初期化ブロックを指すポインタ．システム周
 *  期停止モードの場合は，NULLにする．
 */
extern const SOMINIB	*p_cursom;

/*
 *  次のシステム動作モード
 *
 *  現在のシステム動作モードがシステム周期停止モードの場合，この変数は
 *  無効である．
 */
extern const SOMINIB	*p_nxtsom;

/*
 *  システム周期を管理するためのタイムイベントブロック
 */
extern TMEVTB	scyc_tmevtb;

/*
 *  システム周期切換え処理を保留していることを示すフラグ
 */
extern bool_t	pending_scycswitch;

/*
 *  実行中のタイムウィンドウ
 *
 *  実行中のタイムウィンドウの初期化ブロックを指すポインタ．システム周
 *  期停止モードの間と，アイドルウィンドウの実行中は，NULLにする．
 */
extern const TWDINIB	*p_runtwd;

/*
 *  タイムウィンドウ切換え処理を保留していることを示すフラグ
 */
extern bool_t	pending_twdswitch;

/*
 *  タイムウィンドウ初期化ブロックのエリア（kernel_cfg.c）
 */
extern const TWDINIB	twdinib_table[];

/*
 *  タイムウィンドウタイマを動作させるべき状態であることを示すフラグ
 */
extern bool_t	twdtimer_enable;

/*
 *  タイムウィンドウタイマが動作中かを示すフラグ
 */
extern bool_t	twdtimer_flag;

/*
 *  タイムウィンドウの残り時間
 *
 *  この変数は，タイムウィンドウタイマを動作させるべき状態
 *  （twdtime_enableがtrue）であり，タイムウィンドウタイマが停止中
 *  （twdtimer_flagがfalseの間）のみ有効である．
 */
extern PRCTIM	left_twdtim;

/*
 *  システム動作モードIDの最大値（kernel_cfg.c）
 */
extern const ID		tmax_somid;

/*
 *  初期システム動作モード（kernel_cfg.c）
 *
 *  初期システム動作モードが設定されていない時は，NULLにする．
 */
extern const SOMINIB	*const p_inisom;

/*
 *  システム動作モード初期化ブロックのエリア（kernel_cfg.c）
 */
extern const SOMINIB	sominib_table[];

/*
 *  システム動作モードIDからシステム動作モード初期化ブロックを取り出す
 *  ためのマクロ
 */
#define INDEX_SOM(somid)	((uint_t)((somid) - TMIN_SOMID))
#define get_sominib(somid)	(&(sominib_table[INDEX_SOM(somid)]))

/*
 *  システム動作モード初期化ブロックからシステム動作モードIDを取り出す
 *  ためのマクロ
 */
#define	SOMID(p_sominib)	((ID)(((p_sominib) - sominib_table) + TMIN_SOMID))

/*
 *  保護ドメイン管理ブロック
 *
 *  保護ドメイン管理ブロックを参照することが少ないため，他の管理ブロッ
 *  クと初期化ブロックの関係とは逆に，保護ドメイン初期化ブロックに保護
 *  ドメイン管理ブロックへのポインタを持たせている．
 */
typedef struct domain_control_block {
	QUEUE		free_tcb;		/* 未使用のTCB */
	QUEUE		free_semcb;		/* 未使用のセマフォ管理ブロック */
	QUEUE		free_flgcb;		/* 未使用のイベントフラグ管理ブロック */
	QUEUE		free_dtqcb;		/* 未使用のデータキュー管理ブロック */
	QUEUE		free_pdqcb;		/* 未使用の優先度データキュー管理ブロック */
	QUEUE		free_mtxcb;		/* 未使用のミューテックス管理ブロック */
	QUEUE		free_mbfcb;		/* 未使用のメッセージバッファ管理ブロック */
	QUEUE		free_mpfcb;		/* 未使用の固定長メモリプール管理ブロック */
	QUEUE		free_cyccb;		/* 未使用の周期通知管理ブロック */
	QUEUE		free_almcb;		/* 未使用のアラーム通知管理ブロック */
	bool_t		mpk_valid;		/* カーネルメモリプール領域有効フラグ */
} DOMCB;

/*
 *  保護ドメイン初期化ブロック
 */
struct domain_initialization_block {
	ACPTN		domptn;			/* 保護ドメインのビットパターン */
	SCHEDCB		*p_schedcb;		/* スケジューリング単位管理ブロック */
	TMEVTN		*p_tmevt_heap;	/* タイムイベントヒープ */
	uint_t		minpriority;	/* 設定できるタスク優先度の最小値 */
#ifdef USE_DOMINICTXB
	DOMINICTXB	domctxb;		/* 保護ドメイン初期化コンテキストブロック */
#endif /* USE_DOMINICTXB */
	ACVCT		acvct;			/* アクセス許可ベクタ */

	DOMCB		*p_domcb;		/* 管理ブロックへのポインタ */
	uint_t		tnum_atskid;	/* 割付け可能なタスクIDの数 */
	uint_t		tnum_asemid;	/* 割付け可能なセマフォIDの数 */
	uint_t		tnum_aflgid;	/* 割付け可能なイベントフラグIDの数 */
	uint_t		tnum_adtqid;	/* 割付け可能なデータキューIDの数 */
	uint_t		tnum_apdqid;	/* 割付け可能な優先度データキューIDの数 */
	uint_t		tnum_amtxid;	/* 割付け可能なミューテックスIDの数 */
	uint_t		tnum_ambfid;	/* 割付け可能なメッセージバッファIDの数 */
	uint_t		tnum_ampfid;	/* 割付け可能な固定長メモリプールIDの数 */
	uint_t		tnum_acycid;	/* 割付け可能な周期通知IDの数 */
	uint_t		tnum_aalmid;	/* 割付け可能なアラーム通知IDの数 */
	size_t		mpksz;			/* カーネルメモリプール領域のサイズ */
	MB_T		*mpk;			/* カーネルメモリプール領域の先頭番地 */
};

/*
 *  実行中の保護ドメインのビットパターン
 *
 *  実行中の保護ドメインの情報を，ビットパターンの形で持つ変数．タスク
 *  が拡張サービスコールを呼び出していない時には，
 *  p_runtsk->p_dominib->domptnに設定し，非タスクコンテキスト実行中と，
 *  タスクが拡張サービスコールを呼び出している時は，TACP_KERNELに設定す
 *  る．
 */
extern ACPTN	rundom;

/*
 *  MMU/MPUに設定されている保護ドメイン
 */
extern const DOMINIB	*p_ctxdom;

/*
 *  保護ドメインIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_domid;

/*
 *  保護ドメイン管理ブロックのエリア（kernel_cfg.c）
 */
extern DOMCB	domcb_table[];

/*
 *  カーネルドメインの保護ドメイン初期化ブロック（kernel_cfg.c）
 */
extern const DOMINIB	dominib_kernel;

/*
 *  保護ドメイン初期化ブロックのエリア（kernel_cfg.c）
 */
extern const DOMINIB	dominib_table[];

/*
 *  無所属に対する保護ドメイン初期化ブロック（kernel_cfg.c）
 */
extern const DOMINIB	dominib_none;

/*
 *  ユーザドメインの数
 */
#define tnum_udom	((uint_t)(tmax_domid - TMIN_DOMID + 1))

/*
 *  保護ドメインIDから保護ドメイン初期化ブロックを取り出すためのマクロ
 */
#define INDEX_DOM(domid)	((uint_t)((domid) - TMIN_DOMID))
#define get_dominib(domid)	(&(dominib_table[INDEX_DOM(domid)]))

/*
 *  オブジェクト属性中のドメインIDを操作するためのマクロ
 */
#define TA_DOMMASK					(0xffU << 16)
#define get_atrdomid(atr)			((ID)(int8_t)((atr) >> 16))
#define set_atrdomid(atr, domid)	(((atr) & ~TA_DOMMASK) | (domid))

/*
 *  オブジェクト属性から保護ドメイン初期化ブロックを取り出す
 */
Inline const DOMINIB *
get_atrdominib(ATR atr)
{
	ID	domid;

	domid = get_atrdomid(atr);
	switch (domid) {
	case TDOM_KERNEL:
		return(&dominib_kernel);
	case TDOM_NONE:
		return(&dominib_none);
	default:
		return(get_dominib(domid));
	}
}

/*
 *  デフォルトのアクセス許可パターンを取り出すマクロ
 */
#define default_acptn(domid)	((domid) == TDOM_KERNEL ? TACP_KERNEL	\
								: (domid) == TDOM_NONE ? TACP_SHARED	\
								: TACP(domid))

/*
 *  保護ドメイン管理モジュールの初期化
 */
extern void		initialize_domain(void);

/*
 *  タイムウィンドウタイマの動作開始
 */
extern void		twdtimer_start(void);

/*
 *  タイムウィンドウタイマの停止
 */
extern void		twdtimer_stop(void);

/*
 *  タイムウィンドウタイマの切換え
 */
extern void		twdtimer_control(void);

/*
 *  システム周期の実行開始
 */
extern void		scyc_start(void);

/*
 *  システム周期切換え処理
 */
extern void		scyc_switch(void);

/*
 *  タイムウィンドウの実行開始
 */
extern void		twd_start(void);

/*
 *  タイムウィンドウ切換え処理
 */
extern void		twd_switch(void);

/*
 *  タスクディスパッチ可能状態への遷移
 *
 *  タスクディスパッチ可能状態であることを示すフラグ（dspflg）をtrueに
 *  し，保留された切換え処理を実行する．また，実行すべきタスクを更新す
 *  る．
 */
extern void		set_dspflg(void);

#endif /* TOPPERS_DOMAIN_H_ */
