/* This file is generated from chip_rename.def by genrename. */

/* This file is included only when chip_rename.h has been included. */
#ifdef TOPPERS_CHIP_RENAME_H
#undef TOPPERS_CHIP_RENAME_H

/*
 *  chip_kernel_impl.c
 */
#undef chip_initialize
#undef chip_terminate

/*
 *  chip_timer.c
 */
#undef target_hrt_initialize
#undef target_hrt_terminate
#undef target_hrt_handler
#undef target_twdtimer_initialize
#undef target_twdtimer_terminate
#undef target_twdtimer_handler


#include "core_unrename.h"

#endif /* TOPPERS_CHIP_RENAME_H */
