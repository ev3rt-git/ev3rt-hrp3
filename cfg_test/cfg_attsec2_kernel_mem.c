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
extern char __start_mo_text_kernel_0_S[];
extern char __start_mo_rodata_kernel[];
extern char __start_mo_rodata_kernel_0_S[];
extern char __start_mo_text_kernel_S[];
extern char __start_mo_text_kernel_S_S[];
extern char __start_mo_rodata_kernel_S[];
extern char __start_mo_rodata_kernel_S_S[];
extern char __aend_mo_rodata_kernel_S_S[];
extern char __start_mo_rwdata_kernel_S_0[];
extern char __start_mo_rwdata_kernel_S_0_S[];
extern char __start_mo_rwdata_kernel_S_S[];
extern char __start_mo_rwdata_kernel_S_S_S[];
extern char __start_mo_rwdata_kernel[];
extern char __start_mo_rwdata_kernel_0_0_S[];
extern char __start_mo_srpw_kernel[];
extern char __start_mo_srpw_kernel_S[];
extern char __aend_mo_srpw_kernel_S[];

const uint_t _kernel_tnum_meminib = 19U;

void *const _kernel_memtop_table[19] = {
	0,
	__start_mo_text_kernel /* 0x01000000 */,
	__start_mo_text_kernel_0_S /* 0x01009a7c */,
	__start_mo_rodata_kernel /* 0x0100a000 */,
	__start_mo_rodata_kernel_0_S /* 0x0100a784 */,
	__start_mo_text_kernel_S /* 0x0100b000 */,
	__start_mo_text_kernel_S_S /* 0x0100b008 */,
	__start_mo_rodata_kernel_S /* 0x0100c000 */,
	__start_mo_rodata_kernel_S_S /* 0x0100c008 */,
	__aend_mo_rodata_kernel_S_S /* 0x0100d000 */,
	__start_mo_rwdata_kernel_S_0 /* 0x01800000 */,
	__start_mo_rwdata_kernel_S_0_S /* 0x0180000c */,
	__start_mo_rwdata_kernel_S_S /* 0x01801000 */,
	__start_mo_rwdata_kernel_S_S_S /* 0x0180100c */,
	__start_mo_rwdata_kernel /* 0x01802000 */,
	__start_mo_rwdata_kernel_0_0_S /* 0x018041e8 */,
	__start_mo_srpw_kernel /* 0x01805000 */,
	__start_mo_srpw_kernel_S /* 0x0180500c */,
	__aend_mo_srpw_kernel_S /* 0x01806000 */
};

const MEMINIB _kernel_meminib_table[19] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_SHARED },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_SHARED },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_KERNEL },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_RODATASEC, 0U, TACP_SHARED, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x0, TACP_SHARED, TACP_KERNEL, TACP_KERNEL },
	{ 0x0, TACP_SHARED, TACP_KERNEL, TACP_SHARED },
	{ 0x0, TACP_SHARED, TACP_SHARED, TACP_KERNEL },
	{ 0x0, TACP_SHARED, TACP_SHARED, TACP_SHARED },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_SHARED },
	{ 0x0, TACP_KERNEL, TACP_SHARED, TACP_KERNEL },
	{ 0x0, TACP_KERNEL, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U }
};

/*
 *  Data Section Initialization Information
 */

extern char __start_data_kernel_S_0[];
extern char __end_data_kernel_S_0[];
extern char __start_idata_kernel_S_0[];
extern char __start_data_kernel_S_0_S[];
extern char __end_data_kernel_S_0_S[];
extern char __start_idata_kernel_S_0_S[];
extern char __start_data_kernel_S_S[];
extern char __end_data_kernel_S_S[];
extern char __start_idata_kernel_S_S[];
extern char __start_data_kernel_S_S_S[];
extern char __end_data_kernel_S_S_S[];
extern char __start_idata_kernel_S_S_S[];
extern char __start_data_kernel[];
extern char __end_data_kernel[];
extern char __start_idata_kernel[];
extern char __start_data_kernel_0_0_S[];
extern char __end_data_kernel_0_0_S[];
extern char __start_idata_kernel_0_0_S[];
extern char __start_data_kernel_0_S[];
extern char __end_data_kernel_0_S[];
extern char __start_idata_kernel_0_S[];
extern char __start_data_kernel_0_S_S[];
extern char __end_data_kernel_0_S_S[];
extern char __start_idata_kernel_0_S_S[];

const uint_t _kernel_tnum_datasec = 8U;

const DATASECINIB _kernel_datasecinib_table[8] = {
	{ __start_data_kernel_S_0, __end_data_kernel_S_0, __start_idata_kernel_S_0 },
	{ __start_data_kernel_S_0_S, __end_data_kernel_S_0_S, __start_idata_kernel_S_0_S },
	{ __start_data_kernel_S_S, __end_data_kernel_S_S, __start_idata_kernel_S_S },
	{ __start_data_kernel_S_S_S, __end_data_kernel_S_S_S, __start_idata_kernel_S_S_S },
	{ __start_data_kernel, __end_data_kernel, __start_idata_kernel },
	{ __start_data_kernel_0_0_S, __end_data_kernel_0_0_S, __start_idata_kernel_0_0_S },
	{ __start_data_kernel_0_S, __end_data_kernel_0_S, __start_idata_kernel_0_S },
	{ __start_data_kernel_0_S_S, __end_data_kernel_0_S_S, __start_idata_kernel_0_S_S }
};

/*
 *  BSS Section Initialization Information
 */

extern char __start_bss_kernel_S_0[];
extern char __end_bss_kernel_S_0[];
extern char __start_bss_kernel_S_0_S[];
extern char __end_bss_kernel_S_0_S[];
extern char __start_bss_kernel_S_S[];
extern char __end_bss_kernel_S_S[];
extern char __start_bss_kernel_S_S_S[];
extern char __end_bss_kernel_S_S_S[];
extern char __start_bss_kernel[];
extern char __end_bss_kernel[];
extern char __start_bss_kernel_0_0_S[];
extern char __end_bss_kernel_0_0_S[];
extern char __start_bss_kernel_0_S[];
extern char __end_bss_kernel_0_S[];
extern char __start_bss_kernel_0_S_S[];
extern char __end_bss_kernel_0_S_S[];

const uint_t _kernel_tnum_bsssec = 8U;

const BSSSECINIB _kernel_bsssecinib_table[8] = {
	{ __start_bss_kernel_S_0, __end_bss_kernel_S_0 },
	{ __start_bss_kernel_S_0_S, __end_bss_kernel_S_0_S },
	{ __start_bss_kernel_S_S, __end_bss_kernel_S_S },
	{ __start_bss_kernel_S_S_S, __end_bss_kernel_S_S_S },
	{ __start_bss_kernel, __end_bss_kernel },
	{ __start_bss_kernel_0_0_S, __end_bss_kernel_0_0_S },
	{ __start_bss_kernel_0_S, __end_bss_kernel_0_S },
	{ __start_bss_kernel_0_S_S, __end_bss_kernel_0_S_S }
};

