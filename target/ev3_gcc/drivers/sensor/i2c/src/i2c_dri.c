#include "kernel/kernel_impl.h"
#include <stdarg.h>
#include <ctype.h>
#include "errno.h"
#include "csl.h"
#include "suart_err.h"
#include "kernel_cfg.h"
#include "kernel/check.h"

#include "driver_debug.h"

#define InitGpio InitGpio_I2C
typedef uint8_t __u8;

//#include  "driver_common.h"

/**
 * Reuse of 'd_iic.c' from LEGO MINDSTORMS EV3 source code
 */
#include "../d_iic/Linuxmod_AM1808/d_iic.c"

INPIN IicPortPin[NO_OF_IIC_PORTS][IIC_PORT_PINS];

static i2c_data_t driver_data_i2c_sensor[TNUM_INPUT_PORT];

void setup_i2c_port(int port) {
	SetGpio(IicPortPin[port][IIC_PORT_BUFFER_CTRL].Pin);
	SetGpio(IicPortPin[port][IIC_PORT_DATA].Pin);
	IicPortEnable(port);
}

static void initialize(intptr_t unused) {
    /*
     *  TIMERP1.TIM12 is already initialized in dual 32-bit timer
     *  unchained mode as twd timer.
     *  TIMERP1.TIM34 is initialized here as I2C timer.
     */
    TIMERP1.TGCR &= ~0xFF02;
    AINTC.SICR = INTNO_I2C_TIMER;
    TIMERP1.TCR &= ~0x40000;
    TIMERP1.TIM34 = 0x0;
    TIMERP1.PRD34 = 0x0;
    TIMERP1.REL34 = 0x0;
    TIMERP1.INTCTLSTAT = 0x30001;
    TIMERP1.TGCR |= 0x2;

	ModuleInit();

	for(int i = 0; i < TNUM_INPUT_PORT; ++i) {
	    driver_data_i2c_sensor[i].raw = IicCtrl.data_package[i].data;
	    driver_data_i2c_sensor[i].status = &(IicCtrl.data_package[i].transfer_state);
	}
	global_brick_info.i2c_sensors = driver_data_i2c_sensor;

#if defined(DEBUG_I2C_SENSOR) || 1
    syslog(LOG_NOTICE, "i2c_dri initialized.");
#endif
}

void initialize_i2c_dri(intptr_t unused) {
	ev3_driver_t driver;
	driver.init_func = initialize;
	driver.softreset_func = NULL;
	SVC_PERROR(platform_register_driver(&driver));
}

ER _start_i2c_transaction(int port, uint_t addr, const uint8_t *writebuf, uint_t writelen, uint_t readlen, ID cdmid) {
	if(!EXTSVC_PROBE_MEM_READ_SIZE(writebuf, writelen)) return E_MACV;

	ER ercd;

	CHECK_SENSOR_PORT(port);
	CHECK_PAR(writelen <= MAX_DEVICE_DATALENGTH);
	CHECK_PAR(readlen <= MAX_DEVICE_DATALENGTH);

	struct IIC_data_package *datapkg = &(IicCtrl.data_package[port]);

	CHECK_OBJ(datapkg->transfer_state == TRANSFER_IDLE);

	datapkg->addr =  addr;
	memcpy(datapkg->data, writebuf, writelen);
	datapkg->write_length     =  writelen;
	datapkg->read_length      =  readlen;
	datapkg->port             =  port;
	datapkg->nacked           =  0;
	datapkg->clock_state      =  1;
	datapkg->transfer_state   =  TRANSFER_START;

	iic_fiq_start_transfer(50,1);

	ercd = E_OK;

error_exit:
	return(ercd);
}

