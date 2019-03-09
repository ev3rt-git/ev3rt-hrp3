/*
 * davinci-iic.c
 *
 *  Created on: Jan 19, 2016
 *      Author: liyixiao
 */

#include "kernel/kernel_impl.h"
#include <stdarg.h>
#include <ctype.h>
#include "errno.h"
#include "csl.h"
#include "suart_err.h"
#include "kernel_cfg.h"
#include "target_timer.h"
#include "driver_debug.h"

typedef uint8_t __u8;
typedef uint64_t u64;
#define   NO_OF_IIC_PORTS             INPUTS

/**
 * Reuse of 'davinci-iic.c' from LEGO MINDSTORMS EV3 source code
 */
#include  "../d_iic/Linuxmod_AM1808/d_iic.h"
extern INPIN IicPortPin[NO_OF_IIC_PORTS][IIC_PORT_PINS];
#include "../d_iic/davinci-iic.c"

void iic_fiq_start_transfer(unsigned int time, bool fiq_nirq) {
	// TODO: should use REAL fiq?
	// Enable timer for one time
	TIMERP1.TIM34 = 0x0;
	TIMERP1.PRD34 = PRCTIM_TO_CMWCNT(time);
	TIMERP1.TCR |= 0x400000;
}

void inthdr_i2c_timer() {
    // Disable timer and clear interrupt
	TIMERP1.TCR &= ~0x400000;
	TIMERP1.INTCTLSTAT = 0x30001;

	if (iic_fiq_handler(NULL)) {
        // Restart timer
		TIMERP1.TIM34 = 0x0;
	    TIMERP1.TCR |= 0x400000;
	}
}
