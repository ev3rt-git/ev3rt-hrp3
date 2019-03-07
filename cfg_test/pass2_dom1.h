/*
 *		静的APIが適切なドメインの囲みの中に記述されていないエラー検出
 *		のテスト(1)
 * 
 *  $Id: pass2_dom1.h 465 2018-09-30 11:03:02Z ertl-hiro $
 */

#include <kernel.h>

/*
 *  優先度の定義
 */
#define MID_PRIORITY	10		/* 中優先度 */

/*
 *  割込み番号，割込みハンドラ番号，CPU例外ハンドラ番号の定義
 */
#define INTNO1		4
#define INTNO2		5
#define INTNO3		6
#define INTNO4		7
#define INHNO1		4
#define INHNO2		5
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
extern STK_T istack[];

/*
 *  関数のプロトタイプ宣言
 */
extern void	task1(intptr_t exinf);
extern void	task2(intptr_t exinf);
extern void	cyclic2_handler(intptr_t exinf);
extern void	cyclic4_handler(intptr_t exinf);
extern void	alarm2_handler(intptr_t exinf);
extern void	alarm4_handler(intptr_t exinf);
extern void isr1(intptr_t exinf);
extern void isr2(intptr_t exinf);
extern void	int1_handler(void);
extern void	int2_handler(void);
extern void	cpuexc1_handler(void *p_excinf);
extern void	cpuexc2_handler(void *p_excinf);
extern ER_UINT	extended_svc1(intptr_t par1, intptr_t par2, intptr_t par3,
								intptr_t par4, intptr_t par5, ID cdmid);
extern ER_UINT	extended_svc2(intptr_t par1, intptr_t par2, intptr_t par3,
								intptr_t par4, intptr_t par5, ID cdmid);
extern void initialize1_routine(intptr_t exinf);
extern void initialize2_routine(intptr_t exinf);
extern void terminate1_routine(intptr_t exinf);
extern void terminate2_routine(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
