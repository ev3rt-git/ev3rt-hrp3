/*
 *		全静的APIのテスト(2)
 * 
 *  $Id: cfg_all2.h 465 2018-09-30 11:03:02Z ertl-hiro $
 */

#include <kernel.h>

/*
 *  優先度の定義
 */
#define HIGH_PRIORITY	9		/* 高優先度 */
#define MID_PRIORITY	10		/* 中優先度 */
#define LOW_PRIORITY	11		/* 低優先度 */

#define MAX_INT_PRIORITY	-1
#define MIN_INT_PRIORITY	TMIN_INTPRI

/*
 *  割込み番号，割込みハンドラ番号，CPU例外ハンドラ番号の定義
 */
#define INTNO1		4
#define INTNO2		5
#define INTNO3		6
#define INHNO1		4
#define CPUEXC1		1
#define CPUEXC2		2

/*
 *  スタックサイズの定義
 */
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  スタック領域，カーネルの管理領域の定義
 */
extern STK_T stack_TASK2[];
extern MPF_T mpf_MPF2[];
extern STK_T istack[];

/*
 *  変数の定義
 */
extern volatile int var1;
extern volatile int var2;

/*
 *  関数のプロトタイプ宣言
 */
extern void	task1(intptr_t exinf);
extern void	task2(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
