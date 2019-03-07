/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2013-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  $Id: pl310.h 172 2016-04-03 01:46:41Z ertl-hiro $
 */

/*
 *		L2キャッシュコントローラ（PL310）に関する定義
 */

#ifndef TOPPERS_PL310_H
#define TOPPERS_PL310_H

/*
 *  型キャストを行うマクロの定義
 */
#ifndef CAST
#define CAST(type, val)		((type)(val))
#endif /* CAST */

/*
 *  PL310のレジスタの番地の定義
 */
#define PL310_CACHE_ID			CAST(uint32_t *, PL310_BASE + 0x000)
#define PL310_CACHE_TYPE		CAST(uint32_t *, PL310_BASE + 0x004)
#define PL310_CTRL				CAST(uint32_t *, PL310_BASE + 0x100)
#define PL310_AUX_CTRL			CAST(uint32_t *, PL310_BASE + 0x104)
#define PL310_TAG_RAM_CTRL		CAST(uint32_t *, PL310_BASE + 0x108)
#define PL310_DATA_RAM_CTRL		CAST(uint32_t *, PL310_BASE + 0x10c)
#define PL310_EVENT_CNT_CTRL	CAST(uint32_t *, PL310_BASE + 0x200)
#define PL310_EVENT_CNT1_CFG	CAST(uint32_t *, PL310_BASE + 0x204)
#define PL310_EVENT_CNT0_CFG	CAST(uint32_t *, PL310_BASE + 0x208)
#define PL310_EVENT_CNT1		CAST(uint32_t *, PL310_BASE + 0x20c)
#define PL310_EVENT_CNT0		CAST(uint32_t *, PL310_BASE + 0x210)
#define PL310_INT_MASK			CAST(uint32_t *, PL310_BASE + 0x214)
#define PL310_INT_MASK_STAT		CAST(uint32_t *, PL310_BASE + 0x218)
#define PL310_INT_RAW_STAT		CAST(uint32_t *, PL310_BASE + 0x21c)
#define PL310_INT_CLEAR			CAST(uint32_t *, PL310_BASE + 0x220)
#define PL310_CACHE_SYNC		CAST(uint32_t *, PL310_BASE + 0x730)
#define PL310_INV_PA			CAST(uint32_t *, PL310_BASE + 0x770)
#define PL310_INV_WAY			CAST(uint32_t *, PL310_BASE + 0x77c)
#define PL310_CLEAN_PA			CAST(uint32_t *, PL310_BASE + 0x7b0)
#define PL310_CLEAN_IDX			CAST(uint32_t *, PL310_BASE + 0x7b8)
#define PL310_CLEAN_WAY			CAST(uint32_t *, PL310_BASE + 0x7bc)
#define PL310_CLEAN_INV_PA		CAST(uint32_t *, PL310_BASE + 0x7f0)
#define PL310_CLEAN_INV_IDX		CAST(uint32_t *, PL310_BASE + 0x7f8)
#define PL310_CLEAN_INV_WAY		CAST(uint32_t *, PL310_BASE + 0x7Fc)
#define PL310_D_LOCKDOWN0		CAST(uint32_t *, PL310_BASE + 0x900)
#define PL310_I_LOCKDOWN0		CAST(uint32_t *, PL310_BASE + 0x904)
#define PL310_DEBUG_CTRL		CAST(uint32_t *, PL310_BASE + 0xf40)
#define PL310_PREFETCH_CTRL		CAST(uint32_t *, PL310_BASE + 0xf60)
#define PL310_POWER_CTRL		CAST(uint32_t *, PL310_BASE + 0xf80)

/*
 *  キャッシュ補助制御レジスタ（PL310_AUX_CTRL）の設定値
 */
#define PL310_AUX_CTRL_ASSOCIATIVITY		0x00010000
#define PL310_AUX_CTRL_WAY_SIZE_SHIFT		17
#define PL310_AUX_CTRL_WAY_SIZE_MASK		0x000e0000
#define PL310_AUX_CTRL_IGNORE_SHARE			0x00400000
#define PL310_AUX_CTRL_NS_LOCKDOWN			0x04000000
#define PL310_AUX_CTRL_NS_INT_CTRL			0x08000000
#define PL310_AUX_CTRL_DATA_PREFETCH		0x10000000
#define PL310_AUX_CTRL_INST_PREFETCH		0x20000000
#define PL310_AUX_CTRL_EARLY_BRESP			0x40000000

/*
 *  プリフェッチ制御レジスタ（PL310_PREFETCH_CTRL）の設定値
 */
#define PL310_PREFETCH_CTRL_INCR_DLINEFILL	0x00800000
#define PL310_PREFETCH_CTRL_DATA_PREFETCH	0x10000000
#define PL310_PREFETCH_CTRL_INST_PREFETCH	0x20000000
#define PL310_PREFETCH_CTRL_DLINEFILL		0x40000000

#ifndef TOPPERS_MACRO_ONLY

/*
 *  L2キャッシュの初期化とイネーブル
 */
extern void pl310_initialize(uint32_t aux_val, uint32_t aux_mask);

/*
 *  L2キャッシュのディスエーブル
 */
extern void pl310_disable(void);

/*
 *  L2キャッシュ全体の無効化（書き戻さない）
 */
extern void pl310_invalidate_all(void);

/*
 *  L2キャッシュ全体のクリーンと無効化
 */
extern void pl310_clean_and_invalidate_all(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_PL310_H */
