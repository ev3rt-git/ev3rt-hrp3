/* 
 *		ATT_SECのテスト(2)
 *
 *  $Id: cfg_attsec2.c 322 2018-04-06 08:47:04Z ertl-hiro $
 */

#include "cfg_attsec.h"

/*
 *  関数の定義
 */
void	task1(intptr_t exinf) { }

void	routine1(void) __attribute__((section(".text_kernel_add1")));
void	routine1(void) { }
void	routine2(void) __attribute__((section(".text_kernel_add2")));
void	routine2(void) { }
void	routine3(void) __attribute__((section(".text_kernel_add3")));
void	routine3(void) { }
void	routine4(void) __attribute__((section(".text_kernel_add4")));
void	routine4(void) { }
void	routine5(void) __attribute__((section(".text_kernel_add5")));
void	routine5(void) { }
void	routine6(void) __attribute__((section(".text_kernel_add6")));
void	routine6(void) { }

const int	const0;
const int	const1 __attribute__((section(".rodata_kernel_add1"),nocommon));
const int	const2 __attribute__((section(".rodata_kernel_add2"),nocommon));
const int	const3 __attribute__((section(".rodata_kernel_add3"),nocommon));
const int	const4 __attribute__((section(".rodata_kernel_add4"),nocommon));
const int	const5 __attribute__((section(".rodata_kernel_add5"),nocommon));
const int	const6 __attribute__((section(".rodata_kernel_add6"),nocommon));

volatile int	var0 = 1;
volatile int	var1 __attribute__((section(".data_kernel_add1"),nocommon)) = 1;
volatile int	var2 __attribute__((section(".data_kernel_add2"),nocommon)) = 2;
volatile int	var3 __attribute__((section(".data_kernel_add3"),nocommon)) = 3;
volatile int	var4 __attribute__((section(".data_kernel_add4"),nocommon)) = 4;
volatile int	var5 __attribute__((section(".data_kernel_add5"),nocommon)) = 5;
volatile int	var6 __attribute__((section(".data_kernel_add6"),nocommon)) = 6;
volatile int	var7 __attribute__((section(".data_kernel_add7"),nocommon)) = 7;
volatile int	var8 __attribute__((section(".data_kernel_add8"),nocommon)) = 8;

volatile int	var10;
volatile int	var11 __attribute__((section(".bss_kernel_add1"),nocommon));
volatile int	var12 __attribute__((section(".bss_kernel_add2"),nocommon));
volatile int	var13 __attribute__((section(".bss_kernel_add3"),nocommon));
volatile int	var14 __attribute__((section(".bss_kernel_add4"),nocommon));
volatile int	var15 __attribute__((section(".bss_kernel_add5"),nocommon));
volatile int	var16 __attribute__((section(".bss_kernel_add6"),nocommon));
volatile int	var17 __attribute__((section(".bss_kernel_add7"),nocommon));
volatile int	var18 __attribute__((section(".bss_kernel_add8"),nocommon));

volatile int	var20;
volatile int	var21 __attribute__((section(".noinit_kernel_add1"),nocommon));
volatile int	var22 __attribute__((section(".noinit_kernel_add2"),nocommon));
volatile int	var23 __attribute__((section(".noinit_kernel_add3"),nocommon));
volatile int	var24 __attribute__((section(".noinit_kernel_add4"),nocommon));
volatile int	var25 __attribute__((section(".noinit_kernel_add5"),nocommon));
volatile int	var26 __attribute__((section(".noinit_kernel_add6"),nocommon));
volatile int	var27 __attribute__((section(".noinit_kernel_add7"),nocommon));
volatile int	var28 __attribute__((section(".noinit_kernel_add8"),nocommon));
