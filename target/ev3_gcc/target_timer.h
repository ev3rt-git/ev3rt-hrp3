#pragma once

#include "kernel/kernel_impl.h"

/*
 * High resolution timer
 */
#if OSCIN_MHZ == 24
#define TCYC_HRTCNT  (178956970U)
#define HRTCNT_BOUND (TCYC_HRTCNT - 2000000U)
#define TSTEP_HRTCNT (1U)
#define HRTCNT_TO_CMWCNT(hrtcnt) ((hrtcnt) * 24U)
#define CMWCNT_TO_HRTCNT(cmwcnt) ((cmwcnt) / 24U)
#define PRCTIM_TO_CMWCNT(prctim) ((prctim) * 24U)
#define CMWCNT_TO_PRCTIM(cmwcnt) ((cmwcnt) / 24U)
#else
#error Must modify definitions above if OSCIN_MHZ is changed.
#endif

#include "chip_timer.h"
