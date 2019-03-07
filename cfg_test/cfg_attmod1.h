/*
 *		ATT_MODのテスト(1)
 *
 *  $Id: cfg_attmod1.h 435 2018-08-12 08:43:50Z ertl-hiro $
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

extern void	task0_1(intptr_t exinf);
extern void	task1_1(intptr_t exinf);
extern void	task2_1(intptr_t exinf);
extern void	task3_1(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
