/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2019 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: kernel.h 672 2019-03-01 12:06:42Z ertl-hiro $
 */

/*
 *		TOPPERS/HRPカーネル 標準ヘッダファイル
 *
 *  TOPPERS/HRPカーネルがサポートするサービスコールの宣言と，必要なデー
 *  タ型，定数，マクロの定義を含むヘッダファイル．
 *
 *  アセンブリ言語のソースファイルからこのファイルをインクルードする時
 *  は，TOPPERS_MACRO_ONLYを定義しておく．これにより，マクロ定義以外を
 *  除くようになっている．
 */

#ifndef TOPPERS_KERNEL_H
#define TOPPERS_KERNEL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	TOPPERS共通のデータ型・定数・マクロ
 */
#include <t_stddef.h>

/*
 *  ターゲット依存部
 */
#include "target_kernel.h"

/*
 *  メモリ配置の方法
 */
#ifdef TOPPERS_ML_MANUAL				/* 手動メモリ配置 */
#ifndef TOPPERS_TARGET_SUPPORT_ML_MANUAL
#error Manual memory layout is not supported on this target.
#endif /* TOPPERS_TARGET_SUPPORT_ML_MANUAL */
#else /* TOPPERS_ML_MANUAL */
#ifndef TOPPERS_TARGET_SUPPORT_ML_AUTO
#error Automatic memory layout is not supported on this target.
#endif /* TOPPERS_TARGET_SUPPORT_ML_AUTO */
#define TOPPERS_ML_AUTO					/* 自動メモリ配置 */
#endif /* TOPPERS_ML_MANUAL */

/*
 *  サポートする機能
 */
#ifdef TOPPERS_TARGET_SUPPORT_DIS_INT
#define TOPPERS_SUPPORT_DIS_INT			/* dis_intがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_DIS_INT */

#ifdef TOPPERS_TARGET_SUPPORT_ENA_INT
#define TOPPERS_SUPPORT_ENA_INT			/* ena_intがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_ENA_INT */

#ifdef TOPPERS_TARGET_SUPPORT_CLR_INT
#define TOPPERS_SUPPORT_CLR_INT			/* clr_intがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_CLR_INT */

#ifdef TOPPERS_TARGET_SUPPORT_RAS_INT
#define TOPPERS_SUPPORT_RAS_INT			/* ras_intがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_RAS_INT */

#ifdef TOPPERS_TARGET_SUPPORT_PRB_INT
#define TOPPERS_SUPPORT_PRB_INT			/* prb_intがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_PRB_INT */

#define TOPPERS_SUPPORT_PROTECT			/* 保護機能対応のカーネル */

#define TOPPERS_SUPPORT_MESSAGEBUF		/* メッセージバッファ機能 */

#ifdef TOPPERS_TARGET_SUPPORT_ATT_MOD
#ifdef TOPPERS_ML_AUTO
#define TOPPERS_SUPPORT_ATT_MOD			/* ATT_MODがサポートされている */
#endif /* TOPPERS_ML_AUTO */
#endif /* TOPPERS_TARGET_SUPPORT_ATT_MOD */

#ifdef TOPPERS_TARGET_SUPPORT_ATT_PMA
#define TOPPERS_SUPPORT_ATT_PMA			/* ATT_PMAがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_ATT_PMA */

#define TOPPERS_SUPPORT_DYNAMIC_CRE		/* 動的生成機能拡張 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  データ型の定義
 */

/*
 *  ビットパターンやオブジェクト番号の型定義
 */
typedef	uint_t		FLGPTN;		/* イベントフラグのビットパターン */
typedef	uint_t		INTNO;		/* 割込み番号 */
typedef	uint_t		INHNO;		/* 割込みハンドラ番号 */
typedef	uint_t		EXCNO;		/* CPU例外ハンドラ番号 */

/*
 *  処理単位の型定義
 */
typedef void	(*TASK)(intptr_t exinf);
typedef void	(*TMEHDR)(intptr_t exinf);
typedef void	(*ISR)(intptr_t exinf);
typedef void	(*INTHDR)(void);
typedef void	(*EXCHDR)(void *p_excinf);
typedef ER_UINT	(*EXTSVC)(intptr_t par1, intptr_t par2, intptr_t par3,
								intptr_t par4, intptr_t par5, ID cdmid);
typedef void	(*INIRTN)(intptr_t exinf);
typedef void	(*TERRTN)(intptr_t exinf);

/*
 *  メモリ領域確保のための型定義
 */
#ifndef TOPPERS_STK_T
#define TOPPERS_STK_T	intptr_t
#endif /* TOPPERS_STK_T */
typedef	TOPPERS_STK_T	STK_T;	/* スタック領域を確保するための型 */

#ifndef TOPPERS_MPF_T
#define TOPPERS_MPF_T	intptr_t
#endif /* TOPPERS_MPF_T */
typedef	TOPPERS_MPF_T	MPF_T;	/* 固定長メモリプール領域を確保するための型 */

/*
 *  タイムイベントの通知方法のパケット形式の定義
 */
typedef struct {
	intptr_t	exinf;		/* タイムイベントハンドラの拡張情報 */
	TMEHDR		tmehdr;		/* タイムイベントハンドラの先頭番地 */
} T_NFY_HDR;

typedef struct {
	intptr_t	*p_var;		/* 変数の番地 */
	intptr_t	value;		/* 設定する値 */
} T_NFY_VAR;

typedef struct {
	intptr_t	*p_var;		/* 変数の番地 */
} T_NFY_IVAR;

typedef struct {
	ID			tskid;		/* タスクID */
} T_NFY_TSK;

typedef struct {
	ID			semid;		/* セマフォID */
} T_NFY_SEM;

typedef struct {
	ID			flgid;		/* イベントフラグID */
	FLGPTN		flgptn;		/* セットするビットパターン */
} T_NFY_FLG;

typedef struct {
	ID			dtqid;		/* データキューID */
	intptr_t	data;		/* 送信する値 */
} T_NFY_DTQ;

typedef struct {
	intptr_t	*p_var;		/* 変数の番地 */
} T_ENFY_VAR;

typedef struct {
	ID			dtqid;		/* データキューID */
} T_ENFY_DTQ;

typedef struct {
	MODE	nfymode;			/* 通知処理モード */
	union {						/* タイムイベントの通知に関する付随情報 */
		T_NFY_HDR	handler;
		T_NFY_VAR	setvar;		
		T_NFY_IVAR	incvar;		
		T_NFY_TSK	acttsk;
		T_NFY_TSK	wuptsk;
		T_NFY_SEM	sigsem;
		T_NFY_FLG	setflg;
		T_NFY_DTQ	snddtq;
	} nfy;
	union {						/* エラーの通知に関する付随情報 */
		T_ENFY_VAR	setvar;
		T_NFY_IVAR	incvar;		
		T_NFY_TSK	acttsk;
		T_NFY_TSK	wuptsk;
		T_NFY_SEM	sigsem;
		T_NFY_FLG	setflg;
		T_ENFY_DTQ	snddtq;
	} enfy;
} T_NFYINFO;

/*
 *  パケット形式の定義
 */
typedef struct t_ctsk {
	ATR			tskatr;		/* タスク属性 */
	intptr_t	exinf;		/* タスクの拡張情報 */
	TASK		task;		/* タスクのメインルーチンの先頭番地 */
	PRI			itskpri;	/* タスクの起動時優先度 */
	size_t		stksz;		/* タスクのスタック領域のサイズ */
	STK_T		*stk;		/* タスクのスタック領域の先頭番地 */
	size_t		sstksz;		/* タスクのシステムスタック領域のサイズ */
	STK_T		*sstk;		/* タスクのシステムスタック領域の先頭番地 */
} T_CTSK;

typedef struct t_rtsk {
	STAT	tskstat;	/* タスク状態 */
	PRI		tskpri;		/* タスクの現在優先度 */
	PRI		tskbpri;	/* タスクのベース優先度 */
	STAT	tskwait;	/* 待ち要因 */
	ID		wobjid;		/* 待ち対象のオブジェクトのID */
	TMO		lefttmo;	/* タイムアウトするまでの時間 */
	uint_t	actcnt;		/* 起動要求キューイング数 */
	uint_t	wupcnt;		/* 起床要求キューイング数 */
	bool_t	raster;		/* タスク終了要求状態 */
	bool_t	dister;		/* タスク終了禁止状態 */
	uint_t	svclevel;	/* 拡張サービスコールのネストレベル */
} T_RTSK;

typedef struct t_csem {
	ATR		sematr;		/* セマフォ属性 */
	uint_t	isemcnt;	/* セマフォの初期資源数 */
	uint_t	maxsem;		/* セマフォの最大資源数 */
} T_CSEM;

typedef struct t_rsem {
	ID		wtskid;		/* セマフォの待ち行列の先頭のタスクのID番号 */
	uint_t	semcnt;		/* セマフォの現在の資源数 */
} T_RSEM;

typedef struct t_cflg {
	ATR		flgatr;		/* イベントフラグ属性 */
	FLGPTN	iflgptn;	/* イベントフラグの初期ビットパターン */
} T_CFLG;

typedef struct t_rflg {
	ID		wtskid;		/* イベントフラグの待ち行列の先頭のタスクのID番号 */
	FLGPTN	flgptn;		/* イベントフラグの現在のビットパターン */
} T_RFLG;

typedef struct t_cdtq {
	ATR		dtqatr;		/* データキュー属性 */
	uint_t	dtqcnt;		/* データキュー管理領域に格納できるデータ数 */
	void	*dtqmb;		/* データキュー管理領域の先頭番地 */
} T_CDTQ;

typedef struct t_rdtq {
	ID		stskid;		/* データキューの送信待ち行列の先頭のタスクのID番号 */
	ID		rtskid;		/* データキューの受信待ち行列の先頭のタスクのID番号 */
	uint_t	sdtqcnt;	/* データキュー管理領域に格納されているデータの数 */
} T_RDTQ;

typedef struct t_cpdq {
	ATR		pdqatr;		/* 優先度データキュー属性 */
	uint_t	pdqcnt;		/* 優先度データキュー管理領域に格納できるデータ数 */
	PRI		maxdpri;	/* 優先度データキューに送信できるデータ優先度の最
						   大値 */
	void	*pdqmb;		/* 優先度データキュー管理領域の先頭番地 */
} T_CPDQ;

typedef struct t_rpdq {
	ID		stskid;		/* 優先度データキューの送信待ち行列の先頭のタスク
						   のID番号 */
	ID		rtskid;		/* 優先度データキューの受信待ち行列の先頭のタスク
						   のID番号 */
	uint_t	spdqcnt;	/* 優先度データキュー管理領域に格納されているデー
						   タの数 */
} T_RPDQ;

typedef struct t_cmtx {
	ATR		mtxatr;		/* ミューテックス属性 */
	PRI		ceilpri;	/* ミューテックスの上限優先度 */
} T_CMTX;

typedef struct t_rmtx {
	ID		htskid;		/* ミューテックスをロックしているタスクのID番号 */
	ID		wtskid;		/* ミューテックスの待ち行列の先頭のタスクのID番号 */
} T_RMTX;

typedef struct t_cmbf {
	ATR		mbfatr;		/* メッセージバッファ属性 */
	uint_t	maxmsz;		/* メッセージバッファの最大メッセージ
							   サイズ（バイト数）*/
	size_t	mbfsz;		/* メッセージバッファ管理領域のサイズ
							   （バイト数）*/
	void	*mbfmb;		/* メッセージバッファ管理領域の先頭番地 */
} T_CMBF;

typedef struct t_rmbf {
	ID		stskid;		/* メッセージバッファの送信待ち行列の先頭のタスク
						   のID番号 */
	ID		rtskid;		/* メッセージバッファの受信待ち行列の先頭のタスク
						   のID番号 */
	uint_t	smbfcnt;	/* メッセージバッファ管理領域に格納されているメッ
						   セージの数 */
	size_t	fmbfsz;		/* メッセージバッファ管理領域中の空き領域のサイズ */
} T_RMBF;

typedef struct t_cmpf {
	ATR		mpfatr;		/* 固定長メモリプール属性 */
	uint_t	blkcnt;		/* 獲得できる固定長メモリブロックの数 */
	uint_t	blksz;		/* 固定長メモリブロックのサイズ */
	MPF_T	*mpf;		/* 固定長メモリプール領域の先頭番地 */
	void	*mpfmb;		/* 固定長メモリプール管理領域の先頭番地 */
} T_CMPF;

typedef struct t_rmpf {
	ID		wtskid;		/* 固定長メモリプールの待ち行列の先頭のタスクの
						   ID番号 */
	uint_t	fblkcnt;	/* 固定長メモリプール領域の空きメモリ領域に割り
						   付けることができる固定長メモリブロックの数 */
} T_RMPF;

typedef struct t_ccyc {
	ATR			cycatr;		/* 周期通知属性 */
	T_NFYINFO	nfyinfo;	/* 周期通知の通知方法 */
	RELTIM		cyctim;		/* 周期通知の通知周期 */
	RELTIM		cycphs;		/* 周期通知の通知位相 */
} T_CCYC;

typedef struct t_rcyc {
	STAT	cycstat;	/* 周期通知の動作状態 */
	RELTIM	lefttim;	/* 次回通知時刻までの相対時間 */
} T_RCYC;

typedef struct t_calm {
	ATR			almatr;		/* アラーム通知属性 */
	T_NFYINFO	nfyinfo;	/* アラーム通知の通知方法 */
} T_CALM;

typedef struct t_ralm {
	STAT	almstat;	/* アラーム通知の動作状態 */
	RELTIM	lefttim;	/* 通知時刻までの相対時間 */
} T_RALM;

typedef struct t_cisr {
	ATR			isratr;		/* 割込みサービスルーチン属性 */
	intptr_t	exinf;		/* 割込みサービスルーチンの拡張情報 */
	INTNO		intno;		/* 割込みサービスルーチンを登録する割込み番号 */
	ISR			isr;		/* 割込みサービスルーチンの先頭番地 */
	PRI			isrpri;		/* 割込みサービスルーチン優先度 */
} T_CISR;

typedef struct t_rmem {
	ATR		accatr;		/* メモリオブジェクトアクセス属性 */
	void	*base;		/* メモリオブジェクトの先頭番地 */
	size_t	size;		/* メモリオブジェクトのサイズ（バイト数） */
} T_RMEM;

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  サービスコール呼出しのための定義と宣言
 */
#include <kernel_fncode.h>				/* 機能コードの定義 */
#include "target_svc.h"					/* ソフトウェア割込みによる呼出し */
#include <svc_call.h>					/* 関数呼出しによる呼出し */

#define SVC_CALL(svc)	_kernel_##svc	/* 関数呼出しによる呼び出す名称 */

/*
 *  非タスクコンテキストから呼び出せるサービスコール
 */
#define iact_tsk(tskid)						act_tsk(tskid)
#define iwup_tsk(tskid)						wup_tsk(tskid)
#define irel_wai(tskid)						rel_wai(tskid)
#define isns_ter()							sns_ter()
#define isig_sem(semid)						sig_sem(semid)
#define iset_flg(flgid, setptn)				set_flg(flgid, setptn)
#define ipsnd_dtq(dtqid, data)				psnd_dtq(dtqid, data)
#define ifsnd_dtq(dtqid, data)				fsnd_dtq(dtqid, data)
#define ipsnd_pdq(pdqid, data, datapri)		psnd_pdq(pdqid, data, datapri)
#define ifch_hrt()							fch_hrt()
#define ista_alm(almid, almtim)				sta_alm(almid, almtim)
#define istp_alm(almid)						stp_alm(almid)
#define irot_rdq(tskpri)					rot_rdq(tskpri)
#define imrot_rdq(schedno, tskpri)			mrot_rdq(schedno, tskpri)
#define iget_did(p_domid)					get_did(p_domid)
#define iget_tid(p_tskid)					get_tid(p_tskid)
#define iloc_cpu()							loc_cpu()
#define iunl_cpu()							unl_cpu()
#define isns_ctx()							sns_ctx()
#define isns_loc()							sns_loc()
#define isns_dsp()							sns_dsp()
#define isns_dpn()							sns_dpn()
#define isns_ker()							sns_ker()
#define iext_ker()							ext_ker()
#define idis_int(intno)						dis_int(intno)
#define iena_int(intno)						ena_int(intno)
#define iclr_int(intno)						clr_int(intno)
#define iras_int(intno)						ras_int(intno)
#define iprb_int(intno)						prb_int(intno)
#define ixsns_dpn(p_excinf)					xsns_dpn(p_excinf)

/*
 *  オブジェクト属性の定義
 */
#define TA_DOM(domid)	((((uint_t) domid) & 0xffU) << 16)
										/* 属する保護ドメインの指定 */

#define TA_ACT			UINT_C(0x01)	/* タスクを起動された状態で生成 */
#define TA_NOACTQUE		UINT_C(0x02)	/* 起動要求をキューイングしない */

#define TA_TPRI			UINT_C(0x01)	/* タスクの待ち行列を優先度順に */

#define TA_WMUL			UINT_C(0x02)	/* 複数の待ちタスク */
#define TA_CLR			UINT_C(0x04)	/* イベントフラグのクリア指定 */

#define TA_CEILING		UINT_C(0x03)	/* 優先度上限プロトコル */

#define TA_STA			UINT_C(0x02)	/* 周期通知を動作状態で生成 */

#define TA_NOWRITE		UINT_C(0x01)	/* 書込みアクセス禁止 */
#define TA_NOREAD		UINT_C(0x02)	/* 読出しアクセス禁止 */
#define TA_EXEC			UINT_C(0x04)	/* 実行アクセス許可 */
#define TA_MEMINI		UINT_C(0x08)	/* メモリの初期化を行う */
#define TA_MEMZERO		UINT_C(0x10)	/* メモリのクリアを行う */
#define TA_SDATA		UINT_C(0x20)	/* ショートデータ領域に配置 */
#define TA_UNCACHE		UINT_C(0x40)	/* キャッシュ不可 */
#define TA_IODEV		UINT_C(0x80)	/* 周辺デバイスの領域 */

#define TA_ATTMEM		UINT_C(0x1000)	/* ATT_MEM／ATT_PMAで登録 */
#define TA_USTACK		UINT_C(0x2000)	/* ユーザスタック領域 */

#define TA_NONKERNEL	UINT_C(0x02)	/* カーネル管理外の割込み */

#define TA_ENAINT		UINT_C(0x01)	/* 割込み要求禁止フラグをクリア */
#define TA_EDGE			UINT_C(0x02)	/* エッジトリガ */

#define TA_INISOM		UINT_C(0x01)	/* 初期システム動作モード */

/*
 *  標準のメモリオブジェクト属性［NGKI3963］［NGKI3964］
 */
#ifndef TA_TEXTSEC
#define TA_TEXTSEC			(TA_NOWRITE|TA_EXEC)
#endif /* TA_TEXTSEC */
#ifndef TA_RODATASEC
#define TA_RODATASEC		(TA_NOWRITE)
#endif /* TA_RODATASEC */
#ifndef TA_DATASEC
#define TA_DATASEC			(TA_MEMINI)
#endif /* TA_DATASEC */
#ifndef TA_BSSSEC
#define TA_BSSSEC			(TA_MEMZERO)
#endif /* TA_BSSSEC */
#ifndef TA_NOINITSEC
#define TA_NOINITSEC		(TA_NULL)
#endif /* TA_NOINITSEC */
#ifndef TA_LOSEC
#define TA_LOSEC			(TA_NOWRITE|TA_NOREAD)
#endif /* TA_LOSEC */

/*
 *  サービスコールの動作モードの定義
 */
#define TWF_ORW			UINT_C(0x01)	/* イベントフラグのOR待ち */
#define TWF_ANDW		UINT_C(0x02)	/* イベントフラグのAND待ち */

#define TPM_WRITE		UINT_C(0x01)	/* 書込みアクセス権のチェック */
#define TPM_READ		UINT_C(0x02)	/* 読出しアクセス権のチェック */
#define TPM_EXEC		UINT_C(0x04)	/* 実行アクセス権のチェック */

/*
 *  通知処理モードの定義
 */
#define TNFY_HANDLER	UINT_C(0x00)		/* タイムイベントハンドラの呼出し */
#define TNFY_SETVAR		UINT_C(0x01)		/* 変数の設定 */
#define TNFY_INCVAR		UINT_C(0x02)		/* 変数のインクリメント */
#define TNFY_ACTTSK		UINT_C(0x03)		/* タスクの起動 */
#define TNFY_WUPTSK		UINT_C(0x04)		/* タスクの起床 */
#define TNFY_SIGSEM		UINT_C(0x05)		/* セマフォの資源の返却 */
#define TNFY_SETFLG		UINT_C(0x06)		/* イベントフラグのセット */
#define TNFY_SNDDTQ		UINT_C(0x07)		/* データキューへの送信 */

#define TENFY_SETVAR	UINT_C(0x10)		/* 変数の設定 */
#define TENFY_INCVAR	UINT_C(0x20)		/* 変数のインクリメント */
#define TENFY_ACTTSK	UINT_C(0x30)		/* タスクの起動 */
#define TENFY_WUPTSK	UINT_C(0x40)		/* タスクの起床 */
#define TENFY_SIGSEM	UINT_C(0x50)		/* セマフォの返却 */
#define TENFY_SETFLG	UINT_C(0x60)		/* イベントフラグのセット */
#define TENFY_SNDDTQ	UINT_C(0x70)		/* データキューへの送信 */

/*
 *  オブジェクトの状態の定義
 */
#define TTS_RUN			UINT_C(0x01)	/* 実行状態 */
#define TTS_RDY			UINT_C(0x02)	/* 実行可能状態 */
#define TTS_WAI			UINT_C(0x04)	/* 待ち状態 */
#define TTS_SUS			UINT_C(0x08)	/* 強制待ち状態 */
#define TTS_WAS			UINT_C(0x0c)	/* 二重待ち状態 */
#define TTS_DMT			UINT_C(0x10)	/* 休止状態 */

#define TTW_SLP			UINT_C(0x0001)	/* 起床待ち */
#define TTW_DLY			UINT_C(0x0002)	/* 時間経過待ち */
#define TTW_SEM			UINT_C(0x0004)	/* セマフォの資源獲得待ち */
#define TTW_FLG			UINT_C(0x0008)	/* イベントフラグ待ち */
#define TTW_SDTQ		UINT_C(0x0010)	/* データキューへの送信待ち */
#define TTW_RDTQ		UINT_C(0x0020)	/* データキューからの受信待ち */
#define TTW_SPDQ		UINT_C(0x0100)	/* 優先度データキューへの送信待ち */
#define TTW_RPDQ		UINT_C(0x0200)	/* 優先度データキューからの受信待ち */
#define TTW_MTX			UINT_C(0x0080)	/* ミューテックスのロック待ち状態 */
#define TTW_SMBF		UINT_C(0x0400)	/* メッセージバッファへの送信待ち */
#define TTW_RMBF		UINT_C(0x0800)	/* メッセージバッファからの受信待ち */
#define TTW_MPF			UINT_C(0x2000)	/* 固定長メモリブロックの獲得待ち */

#define TCYC_STP		UINT_C(0x01)	/* 周期通知が動作していない */
#define TCYC_STA		UINT_C(0x02)	/* 周期通知が動作している */

#define TALM_STP		UINT_C(0x01)	/* アラーム通知が動作していない */
#define TALM_STA		UINT_C(0x02)	/* アラーム通知が動作している */

/*
 *  保護ドメインID［NGKI0513］
 */
#define TDOM_SELF		0			/* 自タスクの属する保護ドメイン */
#define TDOM_KERNEL		(-1)		/* カーネルドメイン */
#define TDOM_NONE		(-2)		/* 無所属（保護ドメインに属さない）*/

/*
 *  その他の定数の定義［NGKI0514］
 */
#define TSOM_STP		(-1)		/* システム周期の停止 */

#define TSK_SELF		0			/* 自タスク指定 */
#define TSK_NONE		0			/* 該当するタスクがない */

#define TPRI_SELF		0			/* 自タスクのベース優先度 */
#define TPRI_INI		0			/* タスクの起動時優先度 */

#define TIPM_ENAALL		0			/* 割込み優先度マスク全解除 */

/*
 *  スケジューリング単位番号を構成するマクロ［NGKI0634］
 */
#define SCHEDNO(prcid, domid)	(domid)

/*
 *  構成定数とマクロ
 */

/*
 *  優先度の範囲
 */
#define TMIN_TPRI		1			/* タスク優先度の最小値（最高値）*/
#define TMAX_TPRI		16			/* タスク優先度の最大値（最低値）*/
#define TMIN_DPRI		1			/* データ優先度の最小値（最高値）*/
#define TMAX_DPRI		16			/* データ優先度の最大値（最低値）*/
#define TMIN_ISRPRI		1			/* 割込みサービスルーチン優先度の最小値 */
#define TMAX_ISRPRI		16			/* 割込みサービスルーチン優先度の最大値 */

/*
 *  バージョン情報
 */
#define TKERNEL_MAKER	UINT_C(0x0118)	/* カーネルのメーカーコード */
#define TKERNEL_PRID	UINT_C(0x0006)	/* カーネルの識別番号 */
#define TKERNEL_SPVER	UINT_C(0xf633)	/* カーネル仕様のバージョン番号 */
#define TKERNEL_PRVER	UINT_C(0x3010)	/* カーネルのバージョン番号 */

/*
 *  キューイング回数の最大値
 */
#define TMAX_ACTCNT		UINT_C(1)		/* 起動要求キューイング数の最大値 */
#define TMAX_WUPCNT		UINT_C(1)		/* 起床要求キューイング数の最大値 */

/*
 *  ビットパターンのビット数
 */
#ifndef TBIT_FLGPTN					/* イベントフラグのビット数 */
#define TBIT_FLGPTN		(sizeof(FLGPTN) * CHAR_BIT)
#endif /* TBIT_FLGPTN */

/*
 *  システム時刻の調整できる範囲（単位：μ秒）
 */
#define TMIN_ADJTIM		-1000000		/* システム時刻の最小調整時間 */
#define TMAX_ADJTIM		1000000			/* システム時刻の最大調整時間 */

/*
 *  タイムウィンドウの長さの最大値（単位：μ秒）
 */
#ifndef TMAX_TWDTIM
#define TMAX_TWDTIM		UINT32_MAX
#endif /* TMAX_TWDTIM */

/*
 *  メモリ領域確保のためのマクロ
 *
 *  以下のTOPPERS_COUNT_SZとTOPPERS_ROUND_SZの定義は，unitが2の巾乗であ
 *  ることを仮定している．
 */
#ifndef TOPPERS_COUNT_SZ
#define TOPPERS_COUNT_SZ(sz, unit)	(((sz) + (unit) - 1) / (unit))
#endif /* TOPPERS_COUNT_SZ */
#ifndef TOPPERS_ROUND_SZ
#define TOPPERS_ROUND_SZ(sz, unit)	(((sz) + (unit) - 1) & ~((unit) - 1))
#endif /* TOPPERS_ROUND_SZ */

#define COUNT_STK_T(sz)		TOPPERS_COUNT_SZ(sz, sizeof(STK_T))
#define ROUND_STK_T(sz)		TOPPERS_ROUND_SZ(sz, sizeof(STK_T))

#define COUNT_MB_T(sz)		TOPPERS_COUNT_SZ(sz, sizeof(MB_T))
#define ROUND_MB_T(sz)		TOPPERS_ROUND_SZ(sz, sizeof(MB_T))

#define TSZ_DTQMB(dtqcnt)	(sizeof(intptr_t) * (dtqcnt))
#define TCNT_DTQMB(dtqcnt)	COUNT_MB_T(TSZ_DTQMB(dtqcnt))

#ifndef TSZ_PDQMB
#define TSZ_PDQMB(pdqcnt)	(sizeof(intptr_t) * 3 * (pdqcnt))
#endif /* TSZ_PDQMB */
#define TCNT_PDQMB(pdqcnt)	COUNT_MB_T(TSZ_PDQMB(pdqcnt))

#define TSZ_MBFMB(msgcnt, msgsz) \
	((msgcnt) * (sizeof(uint_t) + TOPPERS_ROUND_SZ(msgsz, sizeof(uint_t))))
#define TCNT_MBFMB(mbfcnt)	COUNT_MB_T(TSZ_MBFMB(mbfcnt))

#define COUNT_MPF_T(blksz)	TOPPERS_COUNT_SZ(blksz, sizeof(MPF_T))
#define ROUND_MPF_T(blksz)	TOPPERS_ROUND_SZ(blksz, sizeof(MPF_T))

#define TSZ_MPFMB(blkcnt)	(sizeof(uint_t) * (blkcnt))
#define TCNT_MPFMB(blkcnt)	COUNT_MB_T(TSZ_MPFMB(blkcnt))

/*
 *  その他の構成定数
 */
#define TMAX_MAXSEM		UINT_MAX	/* セマフォの最大資源数の最大値 */

/*
 *  拡張サービスコールでのメモリアクセス権のチェックのためのマクロ
 *
 *  ローカル変数にcdmidがあることを想定している．
 */
#ifndef EXTSVC_PROBE_MEM_RW
#define EXTSVC_PROBE_MEM_RW(p_var, type)								\
				(ALIGN_TYPE(p_var, type) && (cdmid == TDOM_KERNEL		\
							|| prb_mem((void *)(p_var), sizeof(type),	\
								TSK_SELF, TPM_WRITE|TPM_READ) == E_OK)
#endif /* EXTSVC_PROBE_MEM_RW */

#ifndef EXTSVC_PROBE_MEM_RW_SIZE
#define EXTSVC_PROBE_MEM_RW_SIZE(p_var, size)							\
				(cdmid == TDOM_KERNEL || prb_mem((void *)(p_var), size,	\
								TSK_SELF, TPM_WRITE|TPM_READ) == E_OK)
#endif /* EXTSVC_PROBE_MEM_RW_SIZE */

#ifndef EXTSVC_PROBE_MEM_RW_COUNT
#define EXTSVC_PROBE_MEM_RW_COUNT(p_var, type, count)					\
				(ALIGN_TYPE(p_var, type) && (cdmid == TDOM_KERNEL		\
					|| prb_mem((void *)(p_var), sizeof(type) * (count),	\
								TSK_SELF, TPM_WRITE|TPM_READ) == E_OK)
#endif /* EXTSVC_PROBE_MEM_RW_COUNT */

#ifndef EXTSVC_PROBE_MEM_READ
#define EXTSVC_PROBE_MEM_READ(p_var, type)								\
				(ALIGN_TYPE(p_var, type) && (cdmid == TDOM_KERNEL		\
							|| prb_mem((void *)(p_var), sizeof(type),	\
										TSK_SELF, TPM_READ) == E_OK))
#endif /* EXTSVC_PROBE_MEM_READ */

#ifndef EXTSVC_PROBE_MEM_READ_SIZE
#define EXTSVC_PROBE_MEM_READ_SIZE(p_var, size)							\
				(cdmid == TDOM_KERNEL || prb_mem((void *)(p_var), size,	\
										TSK_SELF, TPM_READ) == E_OK)
#endif /* EXTSVC_PROBE_MEM_READ_SIZE */

#ifndef EXTSVC_PROBE_MEM_READ_COUNT
#define EXTSVC_PROBE_MEM_READ_COUNT(p_var, type, count)					\
				(ALIGN_TYPE(p_var, type) && (cdmid == TDOM_KERNEL		\
					|| prb_mem((void *)(p_var), sizeof(type) * (count),	\
										TSK_SELF, TPM_READ) == E_OK))
#endif /* EXTSVC_PROBE_MEM_READ_COUNT */

#ifndef EXTSVC_PROBE_MEM_WRITE
#define EXTSVC_PROBE_MEM_WRITE(p_var, type)								\
				(ALIGN_TYPE(p_var, type) && (cdmid == TDOM_KERNEL		\
							|| prb_mem((void *)(p_var), sizeof(type),	\
										TSK_SELF, TPM_WRITE) == E_OK))
#endif /* EXTSVC_PROBE_MEM_WRITE */

#ifndef EXTSVC_PROBE_MEM_WRITE_SIZE
#define EXTSVC_PROBE_MEM_WRITE_SIZE(p_var, size)						\
				(cdmid == TDOM_KERNEL || prb_mem((void *)(p_var), size,	\
										TSK_SELF, TPM_WRITE) == E_OK)
#endif /* EXTSVC_PROBE_MEM_WRITE_SIZE */

#ifndef EXTSVC_PROBE_MEM_WRITE_COUNT
#define EXTSVC_PROBE_MEM_WRITE_COUNT(p_var, type, count)				\
				(ALIGN_TYPE(p_var, type) && (cdmid == TDOM_KERNEL		\
					|| prb_mem((void *)(p_var), sizeof(type) * (count),	\
										TSK_SELF, TPM_WRITE) == E_OK))
#endif /* EXTSVC_PROBE_MEM_WRITE_COUNT */

#ifdef __cplusplus
}
#endif

#endif /* TOPPERS_KERNEL_H */
