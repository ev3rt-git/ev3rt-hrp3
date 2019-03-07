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
#include "cfg_all1.h"

/*
 *  Memory Object Initialization Information
 */

extern char __start_mo_text_kernel[];
extern char __start_mo_rodata_kernel[];
extern char __start_mo_text_shared[];
extern char __aend_mo_text_shared[];
extern char __start_mo_rwdata_kernel_S_0_S[];
extern char __start_mo_rwdata_kernel[];
extern char __aend_mo_rwdata_kernel[];

const uint_t _kernel_tnum_meminib = 11U;

void *const _kernel_memtop_table[12] = {
	0,
	__start_mo_text_kernel /* 0x01000000 */,
	__start_mo_rodata_kernel /* 0x0100a000 */,
	__start_mo_text_shared /* 0x0100b000 */,
	__aend_mo_text_shared /* 0x0100c000 */,
	__start_mo_rwdata_kernel_S_0_S /* 0x01800000 */,
	__start_mo_rwdata_kernel /* 0x01801000 */,
	__aend_mo_rwdata_kernel /* 0x01808000 */,
	(void *)(0xf0000000) /* 0xf0000000 */,
	(void *)(0xf1000000) /* 0xf1000000 */,
	(void *)(((char *)(0xf1000000)) + (0x1000000)) /* 0xf2000000 */
};

const MEMINIB _kernel_meminib_table[12] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x0, TACP_SHARED, TACP_KERNEL, TACP_SHARED },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_ATTMEM|(TA_NULL), TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_ATTMEM|(TA_NULL), TACP_SHARED, TACP_KERNEL, TACP_SHARED },
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

extern char __start_bss_kernel_S_0_S[];
extern char __end_bss_kernel_S_0_S[];
extern char __start_bss_kernel[];
extern char __end_bss_kernel[];

const uint_t _kernel_tnum_bsssec = 2U;

const BSSSECINIB _kernel_bsssecinib_table[2] = {
	{ __start_bss_kernel_S_0_S, __end_bss_kernel_S_0_S },
	{ __start_bss_kernel, __end_bss_kernel }
};

