/* This file is generated from chip_rename.def by genrename. */

#ifndef TOPPERS_CHIP_RENAME_H
#define TOPPERS_CHIP_RENAME_H

/*
 *  chip_kernel_impl.c
 */
#define chip_initialize				_kernel_chip_initialize
#define chip_terminate				_kernel_chip_terminate

/*
 *  chip_timer.c
 */
#define target_hrt_initialize		_kernel_target_hrt_initialize
#define target_hrt_terminate		_kernel_target_hrt_terminate
#define target_hrt_handler			_kernel_target_hrt_handler
#define target_totimer_initialize	_kernel_target_totimer_initialize
#define target_totimer_terminate	_kernel_target_totimer_terminate
#define target_totimer_handler		_kernel_target_totimer_handler


#include "core_rename.h"

#endif /* TOPPERS_CHIP_RENAME_H */
