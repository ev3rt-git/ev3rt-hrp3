#pragma once

/**
 * CPU frequency of EV3's processor (AM1808) in MHz
 */
#define CORE_CLK_MHZ (300)

/**
 * Frequency of oscillator input (OSCIN) in MHz
 * OSCIN_MHZ = CORE_CLK_MHZ * (PREDIV + 1)[if enabled]
 *     * (POSTDIV + 1)[if enabled] * (PLLDIV6 + 1)[if enabled]
 *     / (PLLM + 1)
 */
#define OSCIN_MHZ (24)

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    32
#define SIL_DLY_TIM2    16

#if defined(BUILD_EV3_PLATFORM)
#define OMIT_MEMPOOL_DEFAULT
#endif

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */
extern void target_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void target_exit(void) NoReturn;

#endif /* TOPPERS_MACRO_ONLY */

#include "chip_kernel_impl.h"

