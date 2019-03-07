/* 
 *		全静的APIのテスト(1)
 * 
 *  $Id: cfg_all1.c 467 2018-10-07 14:05:30Z ertl-hiro $
 */

#include "cfg_all1.h"

/*
 *  スタック領域，カーネルの管理領域の定義
 */
STK_T	stack_TASK2[COUNT_STK_T(STACK_SIZE)]
				 __attribute__((section(".noinit_kernel"),nocommon));
MPF_T	mpf_MPF2[20 * COUNT_MPF_T(200)]
				 __attribute__((section(".noinit_kernel"),nocommon));
STK_T	istack[COUNT_STK_T(10000)]
				 __attribute__((section(".noinit_kernel"),nocommon));

/*
 *  変数の定義
 */
volatile int	var1;
volatile int	var2 __attribute__((section(".bss_kernel_add2"),nocommon));

/*
 *  関数の定義
 */
void	task1(intptr_t exinf) { }
void	task2(intptr_t exinf) { }
void	cyclic1_handler(intptr_t exinf) { }
void	alarm1_handler(intptr_t exinf) { }
void	isr1(intptr_t exinf) { }
void	isr2(intptr_t exinf) { }
void	isr3(intptr_t exinf) { }
void	int1_handler(void) { }
void	cpuexc1_handler(void *p_excinf) { }
void	cpuexc2_handler(void *p_excinf) { }
ER_UINT	extended_svc1(intptr_t par1, intptr_t par2, intptr_t par3,
				intptr_t par4, intptr_t par5, ID cdmid) { return(E_OK); }
ER_UINT	extended_svc3(intptr_t par1, intptr_t par2, intptr_t par3,
				intptr_t par4, intptr_t par5, ID cdmid) { return(E_OK); }
void	initialize1_routine(intptr_t exinf) { }
void	initialize2_routine(intptr_t exinf) { }
void	terminate1_routine(intptr_t exinf) { }
void	terminate2_routine(intptr_t exinf) { }
