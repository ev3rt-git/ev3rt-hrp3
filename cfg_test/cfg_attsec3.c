/* 
 *		ATT_SECのテスト(3)
 *
 *  $Id: cfg_attsec3.c 467 2018-10-07 14:05:30Z ertl-hiro $
 */

#include "cfg_attsec.h"

/*
 *  関数の定義
 */
void	task1(intptr_t exinf) { }

volatile int	var01 __attribute__((section(".kernel_add01"),nocommon)) = 1;
volatile int	var02 __attribute__((section(".kernel_add02"),nocommon)) = 2;
volatile int	var03 __attribute__((section(".kernel_add03"),nocommon)) = 3;
volatile int	var04 __attribute__((section(".kernel_add04"),nocommon)) = 4;
volatile int	var05 __attribute__((section(".kernel_add05"),nocommon)) = 5;
volatile int	var06 __attribute__((section(".kernel_add06"),nocommon)) = 6;
volatile int	var07 __attribute__((section(".kernel_add07"),nocommon)) = 7;
volatile int	var08 __attribute__((section(".kernel_add08"),nocommon)) = 8;

volatile int	var11 __attribute__((section(".kernel_add11"),nocommon)) = 11;
volatile int	var12 __attribute__((section(".kernel_add12"),nocommon)) = 12;
volatile int	var13 __attribute__((section(".kernel_add13"),nocommon)) = 13;
volatile int	var14 __attribute__((section(".kernel_add14"),nocommon)) = 14;
volatile int	var15 __attribute__((section(".kernel_add15"),nocommon)) = 15;
volatile int	var16 __attribute__((section(".kernel_add16"),nocommon)) = 16;
volatile int	var17 __attribute__((section(".kernel_add17"),nocommon)) = 17;
volatile int	var18 __attribute__((section(".kernel_add18"),nocommon)) = 18;

volatile int	var21 __attribute__((section(".kernel_add21"),nocommon)) = 21;
volatile int	var22 __attribute__((section(".kernel_add22"),nocommon)) = 22;
volatile int	var23 __attribute__((section(".kernel_add23"),nocommon)) = 23;
volatile int	var24 __attribute__((section(".kernel_add24"),nocommon)) = 24;
volatile int	var25 __attribute__((section(".kernel_add25"),nocommon)) = 25;
volatile int	var26 __attribute__((section(".kernel_add26"),nocommon)) = 26;
volatile int	var27 __attribute__((section(".kernel_add27"),nocommon)) = 27;
volatile int	var28 __attribute__((section(".kernel_add28"),nocommon)) = 28;
