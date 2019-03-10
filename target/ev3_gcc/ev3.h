#pragma once

#include <kernel.h>
#include <t_syslog.h>

/**
 * Period in microseconds for the high resolution cyclic
 */
//#define PERIOD_UART1_SIO  (200U)
//#define PERIOD_UART_PORT1 (200U)
//#define PERIOD_UART_PORT2 (200U)
#define PERIOD_UART_SENSOR_CYC (10000U)

/**
 * Interrupt number
 */
#define INTNO_UART_PORT1 (UART1_INT)
#define INTNO_UART_PORT2 (UART0_INT)
#define INTNO_UART_PORT3 (SUART2_INT)
#define INTNO_UART_PORT4 (SUART1_INT)
#define INTNO_I2C_TIMER  (T64P1_TINT34)

/**
 * Interrupt priority
 */
//#define INTPRI_UART_SIO   (TMIN_INTPRI + 1)
#define INTPRI_GPIO       (TMIN_INTPRI)
#define INTPRI_I2C_TIMER  (TMIN_INTPRI)
#define INTPRI_UART_PORT1 (TMIN_INTPRI)
#define INTPRI_UART_PORT2 (TMIN_INTPRI)
#define INTPRI_UART_PORT3 (TMIN_INTPRI)
#define INTPRI_UART_PORT4 (TMIN_INTPRI)
#define INTPRI_EDMA3      (TMIN_INTPRI + 1)
#define INTPRI_USBMSC     (TMIN_INTPRI + 2)
#define INTPRI_TIMER      (TMIN_INTPRI + 3) // TODO: check me? HRP3 proper timer priority?
#define INTPRI_BLUETOOTH  (TMIN_INTPRI + 4)
#define INTPRI_LCD_SPI    (TMIN_INTPRI + 4)

/**
 * Task priority
 */
#define TPRI_INIT_TASK       (TMIN_TPRI)
#define TPRI_PLATFORM_TERM   (TMIN_TPRI)
#define TPRI_USBMSC          (TMIN_TPRI + 1)
#define TPRI_BLUETOOTH_QOS   (TMIN_TPRI + 1)
#define TPRI_BLUETOOTH_HIGH  (TMIN_TPRI + 2)
#define TPRI_APP_TERM_TASK   (TMIN_TPRI + 3) // TODO: move to loader
#define TPRI_EV3_LCD_TASK    (TMIN_TPRI + 3)
#define TPRI_EV3_MONITOR     (TMIN_TPRI + 4)
#define TPRI_PLATFORM_BUSY   (TMIN_TPRI + 5)
#define TPRI_APP_INIT_TASK   (TMIN_TPRI + 6)
#define TPRI_EV3_CYC         (TMIN_TPRI + 7)
#define TMIN_APP_TPRI        (TMIN_TPRI + 8)
#define TPRI_BLUETOOTH_LOW   (TMAX_TPRI)/*(TMIN_TPRI + 1)*/

#if 0 // TODO: put this to proper file
/*
 *  タスクのスタックサイズ
 */
#ifndef STACK_SIZE
#define STACK_SIZE  4096
#endif
#endif

/**
 * Memory
 */
#define KERNEL_HEAP_SIZE (1024 * 1024) //!< Heap size for dynamic memory allocation in TDOM_KERNEL
#define APP_HEAP_SIZE    (1024 * 1024) //!< Heap size for dynamic memory allocation in TDOM_APP

/**
 * Bluetooth configuration
 */
#define BT_SND_BUF_SIZE        (2048)             //!< Size of send buffer
#define BT_HIGH_PRI_TIME_SLICE (1)                //!< Time slice for BT_TSK in high priority mode (mS)
#define BT_LOW_PRI_TIME_SLICE  (19)               //!< Time slice for BT_TSK in low priority mode (mS)
#define BT_USE_EDMA_MODE       (true)            //!< true: EDMA mode, false: interrupt mode

/**
 * Loadable application module configuration (Dynamic loading)
 */
#define TMAX_APP_TSK_NUM     (32)          //!< Maximum number of tasks in a loadable application module
#define TMAX_APP_SEM_NUM     (16)          //!< Maximum number of semaphores in a loadable application module
#define TMAX_APP_FLG_NUM     (16)          //!< Maximum number of event flags in a loadable application module
#define TMAX_APP_DTQ_NUM     (16)          //!< Maximum number of data queues in a loadable application module
#define TMAX_APP_PDQ_NUM     (16)          //!< Maximum number of priority data queues in a loadable application module
#define TMAX_APP_MTX_NUM     (16)          //!< Maximum number of mutexes in a loadable application module
#define TMAX_APP_CYC_NUM     (16)          //!< Maximum number of cyclic handlers in a loadable application module
#define TMAX_APP_TEXT_SIZE   (1024 * 1024) //!< Maximum size of the text section in a loadable application module
#define TMAX_APP_DATA_SIZE   (1024 * 1024) //!< Maximum size of the data section in a loadable application module
#define TMAX_APP_BINARY_SIZE (1024 * 1024) //!< Maximum size of a loadable application module's binary file

/**
 * LCD configuration
 */
#define LCD_FRAME_RATE (25)

/**
 * Miscellaneous configuration
 */
#define FORCE_SHUTDOWN_TIMEOUT (500)  //!< Timeout in milliseconds of force shutdown feature by pressing BACK+LEFT+RIGHT buttons

#ifndef TOPPERS_MACRO_ONLY

/**
 * Default SIO Port for syslog etc.
 */
extern int SIO_PORT_DEFAULT; // TODO: move to platform/?

#endif

/**
 * PRU Soft UART Driver
 */
#define SUART1_INT 3
#define SUART2_INT 4
#ifndef TOPPERS_MACRO_ONLY
extern void pru_suart_isr(intptr_t portline);
#endif

