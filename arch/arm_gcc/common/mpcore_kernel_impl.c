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
 * 
 *  $Id: mpcore_kernel_impl.c 561 2018-11-25 16:13:08Z ertl-hiro $
 */

/*
 *		カーネルのMPCore依存部
 */

#include "kernel_impl.h"
#include <sil.h>
#include "arm.h"

/*
 *  MPCore依存の初期化
 */
void
mpcore_initialize(void)
{
	/*
	 *  キャッシュをディスエーブル
	 */
	arm_disable_cache();

	/*
	 *  コア依存の初期化
	 */
	core_initialize();

	/*
	 *  MPCoreをSMPモードに設定
	 *
	 *  Shareable属性のメモリ領域をキャッシュ有効にするには，シングル
	 *  コアであっても，SMPモードに設定されている必要がある．
	 */
	mpcore_enable_smp();

	/*
	 *  SCUをイネーブル
	 */
	mpcore_enable_scu();

	/*
	 *  キャッシュをイネーブル
	 */
	arm_enable_cache();

	/*
	 * GICのディストリビュータの初期化
	 */
	gicd_initialize();

	/*
	 * GICのCPUインタフェースの初期化
	 */
	gicc_initialize();

	/*
	 *  分岐予測の無効化とイネーブル
	 */
	arm_invalidate_bp();
	arm_enable_bp();
}

/*
 *  MPCore依存の終了処理
 */
void
mpcore_terminate(void)
{
	extern void    software_term_hook(void);
	void (*volatile fp)(void) = software_term_hook;

	/*
	 *  software_term_hookへのポインタを，一旦volatile指定のあるfpに代
	 *  入してから使うのは，0との比較が最適化で削除されないようにするた
	 *  めである．
	 */
	if (fp != 0) {
		(*fp)();
	}

	/*
	 *  GICのCPUインタフェースの終了処理
	 */
	gicc_terminate();

	/*
	 *  GICのディストリビュータの終了処理
	 */
	gicd_terminate();

	/*
	 *  コア依存の終了処理
	 */
	core_terminate();
}
