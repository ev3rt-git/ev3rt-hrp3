/* kernel_mem.c */
#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#if !(TKERNEL_PRID == 0x0006U && (TKERNEL_PRVER & 0xf000U) == 0x3000U)
#error The kernel does not match this configuration file.
#endif

/*
 *  Include Directives
 */

#ifdef TOPPERS_ML_AUTO
#endif
#include "target_timer.h"
#ifdef TOPPERS_SUPPORT_OVRHDR
#endif
#include "cfg_attsec.h"

/*
 *  Memory Object Initialization Information
 */

extern char __start_mo_text_kernel[];
extern char __start_mo_rodata_kernel[];
extern char __start_mo_text_kernel_S_S[];
extern char __aend_mo_text_kernel_S_S[];
extern char __start_mo_rwdata_kernel[];
extern char __aend_mo_rwdata_kernel_0_0_S[];

const uint_t _kernel_tnum_meminib = 7U;

void *const _kernel_memtop_table[7] = {
	0,
	__start_mo_text_kernel /* 0x01000000 */,
	__start_mo_rodata_kernel /* 0x0100a000 */,
	__start_mo_text_kernel_S_S /* 0x0100b000 */,
	__aend_mo_text_kernel_S_S /* 0x0100c000 */,
	__start_mo_rwdata_kernel /* 0x01800000 */,
	__aend_mo_rwdata_kernel_0_0_S /* 0x01803000 */
};

const MEMINIB _kernel_meminib_table[7] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U }
};

/*
 *  Data Section Initialization Information
 */

const uint_t _kernel_tnum_datasec = 0U;

TOPPERS_EMPTY_LABEL(const DATASECINIB, _kernel_datasecinib_table);

/*
 *  BSS Section Initialization Information
 */

extern char __start_bss_kernel[];
extern char __end_bss_kernel[];

const uint_t _kernel_tnum_bsssec = 1U;

const BSSSECINIB _kernel_bsssecinib_table[1] = {
	{ __start_bss_kernel, __end_bss_kernel }
};

