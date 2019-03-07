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
 *  $Id: pl310.c 172 2016-04-03 01:46:41Z ertl-hiro $
 */

/*
 *		L2キャッシュコントローラ（PL310）の操作ライブラリ
 */

#include "kernel_impl.h"
#include "pl310.h"

Inline uint32_t
get_way_mask(void)
{
	uint32_t	aux;
	
	aux = sil_rew_mem(PL310_AUX_CTRL);
	if ((aux & PL310_AUX_CTRL_ASSOCIATIVITY) != 0U) {
		/* 16ウェイ */
		return(0x0000ffffU);
	}
	else {
		/* 8ウェイ */
		return(0x000000ffU);
	}
}

Inline void
pl310_cache_sync(void)
{
	sil_wrw_mem(PL310_CACHE_SYNC, 0);
}

Inline void
pl310_inv_all(void)
{
	uint32_t	way_mask = get_way_mask();

	/* すべてのウェイを無効化する */
	sil_wrw_mem(PL310_INV_WAY, 0xffff);
	while ((sil_rew_mem(PL310_INV_WAY) & way_mask) != 0U) ;
	pl310_cache_sync();
}

Inline void
pl310_debug_set(uint32_t val)
{
	sil_wrw_mem(PL310_DEBUG_CTRL, val);
}

/*
 *  PL310の初期化
 */
void
pl310_initialize(uint32_t aux_val, uint32_t aux_mask)
{
	uint32_t	aux;
	uint32_t	prefetch;
	uint32_t	prefetch_val = 0U;

	/* L2キャッシュがディスエーブルの場合のみ初期化を行う */
	if ((sil_rew_mem(PL310_CTRL) & 0x01) == 0U) {
		/* 共有属性を無視する */
		aux_val |= PL310_AUX_CTRL_IGNORE_SHARE;

		/* 命令プリフェッチをイネーブル */
		aux_val |= PL310_AUX_CTRL_INST_PREFETCH;
		prefetch_val |= PL310_AUX_CTRL_INST_PREFETCH;

		/* データプリフェッチをイネーブル */
		aux_val |= PL310_AUX_CTRL_DATA_PREFETCH;
		prefetch_val |= PL310_AUX_CTRL_DATA_PREFETCH;

		/* ダブルラインフィルをイネーブル */
		prefetch_val |= PL310_PREFETCH_CTRL_DLINEFILL;
		prefetch_val |= PL310_PREFETCH_CTRL_INCR_DLINEFILL;
                
		/* 補助制御レジスタを設定 */
		aux = sil_rew_mem(PL310_AUX_CTRL);
		sil_wrw_mem(PL310_AUX_CTRL, ((aux & aux_mask) | aux_val));

		/* プリフェッチ制御レジスタを設定 */
		prefetch = sil_rew_mem(PL310_PREFETCH_CTRL);
		sil_wrw_mem(PL310_PREFETCH_CTRL, (prefetch | prefetch_val));

		/* L2キャッシュの全体の無効化 */
		pl310_inv_all();
        
		/* クロックゲーティングとスタンバイモードをイネーブル */
		sil_wrw_mem(PL310_POWER_CTRL, 0x03);
        
		/* L2キャッシュのイネーブル */
		sil_wrw_mem(PL310_CTRL, 0x01);
	}    
}

/*
 *  L2キャッシュのディスエーブル
 */
void
pl310_disable(void)
{
	pl310_clean_and_invalidate_all();
	sil_wrw_mem(PL310_CTRL, 0x00);
}

/*
 *  L2キャッシュ全体の無効化（書き戻さない）
 */
void
pl310_invalidate_all(void)
{
	pl310_debug_set(0x03);
	pl310_inv_all();
	pl310_debug_set(0x00);
}

/*
 *  L2キャッシュ全体のクリーンと無効化
 */
void
pl310_clean_and_invalidate_all(void)
{
	uint32_t	way_mask = get_way_mask();

	pl310_debug_set(0x03);
	sil_wrw_mem(PL310_CLEAN_INV_WAY, way_mask);
	while ((sil_rew_mem(PL310_CLEAN_INV_WAY) & way_mask) != 0U) ;
	pl310_cache_sync();
	pl310_debug_set(0x00);
}
