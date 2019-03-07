/*
 *		ATT_SECのテスト(1)(2)
 *
 *  $Id: cfg_attsec.h 322 2018-04-06 08:47:04Z ertl-hiro $
 */

#include <kernel.h>

/*
 *  優先度の定義
 */
#define MID_PRIORITY	10		/* 中優先度 */

/*
 *  スタックサイズの定義
 */
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	task1(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
