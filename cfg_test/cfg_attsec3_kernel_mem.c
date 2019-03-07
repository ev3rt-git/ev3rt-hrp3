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
extern char __start_mo_text_shared[];
extern char __aend_mo_text_shared[];
extern char __start_mo_RAM_text_kernel[];
extern char __start_mo_RAM_rodata_kernel[];
extern char __start_mo_RAM_text_kernel_A7[];
extern char __aend_mo_RAM_text_kernel_A7[];
extern char __start_mo_RAM_text_kernel_A45[];
extern char __start_mo_RAM_rodata_kernel_A41[];
extern char __start_mo_RAM_text_kernel_A47[];
extern char __aend_mo_RAM_text_kernel_A47[];
extern char __start_mo_rwdata_kernel_A4[];
extern char __start_mo_rwdata_kernel[];
extern char __start_mo_rwdata_kernel_A6[];
extern char __start_mo_rwdata_kernel_A2[];
extern char __start_mo_rwdata_kernel_A44[];
extern char __start_mo_rwdata_kernel_A40[];
extern char __start_mo_rwdata_kernel_A46[];
extern char __start_mo_rwdata_kernel_A42[];
extern char __start_mo_RAM_stext_kernel[];
extern char __start_mo_RAM_srodata_kernel[];
extern char __start_mo_RAM_stext_kernel_A7[];
extern char __aend_mo_RAM_stext_kernel_A7[];
extern char __start_mo_srwdata_kernel_A4[];
extern char __start_mo_srwdata_kernel[];
extern char __start_mo_srwdata_kernel_A6[];
extern char __start_mo_srwdata_kernel_A2[];
extern char __aend_mo_srwdata_kernel_A2[];

const uint_t _kernel_tnum_meminib = 30U;

void *const _kernel_memtop_table[30] = {
	0,
	__start_mo_text_kernel /* 0x01000000 */,
	__start_mo_rodata_kernel /* 0x0100a000 */,
	__start_mo_text_shared /* 0x0100b000 */,
	__aend_mo_text_shared /* 0x0100c000 */,
	__start_mo_RAM_text_kernel /* 0x01800000 */,
	__start_mo_RAM_rodata_kernel /* 0x01801000 */,
	__start_mo_RAM_text_kernel_A7 /* 0x01802000 */,
	__aend_mo_RAM_text_kernel_A7 /* 0x01803000 */,
	__start_mo_RAM_text_kernel_A45 /* 0x01804000 */,
	__start_mo_RAM_rodata_kernel_A41 /* 0x01805000 */,
	__start_mo_RAM_text_kernel_A47 /* 0x01806000 */,
	__aend_mo_RAM_text_kernel_A47 /* 0x01807000 */,
	__start_mo_rwdata_kernel_A4 /* 0x01808000 */,
	__start_mo_rwdata_kernel /* 0x01809000 */,
	__start_mo_rwdata_kernel_A6 /* 0x0180c000 */,
	__start_mo_rwdata_kernel_A2 /* 0x0180d000 */,
	__start_mo_rwdata_kernel_A44 /* 0x0180e000 */,
	__start_mo_rwdata_kernel_A40 /* 0x0180f000 */,
	__start_mo_rwdata_kernel_A46 /* 0x01810000 */,
	__start_mo_rwdata_kernel_A42 /* 0x01811000 */,
	__start_mo_RAM_stext_kernel /* 0x01812000 */,
	__start_mo_RAM_srodata_kernel /* 0x01813000 */,
	__start_mo_RAM_stext_kernel_A7 /* 0x01814000 */,
	__aend_mo_RAM_stext_kernel_A7 /* 0x01815000 */,
	__start_mo_srwdata_kernel_A4 /* 0x01816000 */,
	__start_mo_srwdata_kernel /* 0x01817000 */,
	__start_mo_srwdata_kernel_A6 /* 0x01818000 */,
	__start_mo_srwdata_kernel_A2 /* 0x01819000 */,
	__aend_mo_srwdata_kernel_A2 /* 0x0181a000 */
};

const MEMINIB _kernel_meminib_table[30] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_NOWRITE|TA_EXEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOWRITE, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOREAD|TA_NOWRITE|TA_EXEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_NOWRITE|TA_EXEC|TA_UNCACHE, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOWRITE|TA_UNCACHE, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOREAD|TA_NOWRITE|TA_EXEC|TA_UNCACHE, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_EXEC, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOREAD|TA_EXEC, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOREAD, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_EXEC|TA_UNCACHE, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_UNCACHE, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOREAD|TA_EXEC|TA_UNCACHE, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOREAD|TA_UNCACHE, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x5, 0U, TACP_KERNEL, TACP_KERNEL },
	{ 0x1, 0U, TACP_KERNEL, TACP_KERNEL },
	{ 0x7, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x4, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x6, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x2, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U }
};

/*
 *  Data Section Initialization Information
 */

extern char __start_RAM_text_kernel[];
extern char __end_RAM_text_kernel[];
extern char __start_RAM_itext_kernel[];
extern char __start_RAM_rodata_kernel[];
extern char __end_RAM_rodata_kernel[];
extern char __start_RAM_irodata_kernel[];
extern char __start_RAM_text_kernel_A7[];
extern char __end_RAM_text_kernel_A7[];
extern char __start_RAM_itext_kernel_A7[];
extern char __start_RAM_rodata_kernel_A3[];
extern char __end_RAM_rodata_kernel_A3[];
extern char __start_RAM_irodata_kernel_A3[];
extern char __start_RAM_text_kernel_A45[];
extern char __end_RAM_text_kernel_A45[];
extern char __start_RAM_itext_kernel_A45[];
extern char __start_RAM_rodata_kernel_A41[];
extern char __end_RAM_rodata_kernel_A41[];
extern char __start_RAM_irodata_kernel_A41[];
extern char __start_RAM_text_kernel_A47[];
extern char __end_RAM_text_kernel_A47[];
extern char __start_RAM_itext_kernel_A47[];
extern char __start_RAM_rodata_kernel_A43[];
extern char __end_RAM_rodata_kernel_A43[];
extern char __start_RAM_irodata_kernel_A43[];
extern char __start_RAM_stext_kernel[];
extern char __end_RAM_stext_kernel[];
extern char __start_RAM_istext_kernel[];
extern char __start_RAM_srodata_kernel[];
extern char __end_RAM_srodata_kernel[];
extern char __start_RAM_isrodata_kernel[];
extern char __start_RAM_stext_kernel_A27[];
extern char __end_RAM_stext_kernel_A27[];
extern char __start_RAM_istext_kernel_A27[];
extern char __start_RAM_srodata_kernel_A23[];
extern char __end_RAM_srodata_kernel_A23[];
extern char __start_RAM_isrodata_kernel_A23[];

const uint_t _kernel_tnum_datasec = 12U;

const DATASECINIB _kernel_datasecinib_table[12] = {
	{ __start_RAM_text_kernel, __end_RAM_text_kernel, __start_RAM_itext_kernel },
	{ __start_RAM_rodata_kernel, __end_RAM_rodata_kernel, __start_RAM_irodata_kernel },
	{ __start_RAM_text_kernel_A7, __end_RAM_text_kernel_A7, __start_RAM_itext_kernel_A7 },
	{ __start_RAM_rodata_kernel_A3, __end_RAM_rodata_kernel_A3, __start_RAM_irodata_kernel_A3 },
	{ __start_RAM_text_kernel_A45, __end_RAM_text_kernel_A45, __start_RAM_itext_kernel_A45 },
	{ __start_RAM_rodata_kernel_A41, __end_RAM_rodata_kernel_A41, __start_RAM_irodata_kernel_A41 },
	{ __start_RAM_text_kernel_A47, __end_RAM_text_kernel_A47, __start_RAM_itext_kernel_A47 },
	{ __start_RAM_rodata_kernel_A43, __end_RAM_rodata_kernel_A43, __start_RAM_irodata_kernel_A43 },
	{ __start_RAM_stext_kernel, __end_RAM_stext_kernel, __start_RAM_istext_kernel },
	{ __start_RAM_srodata_kernel, __end_RAM_srodata_kernel, __start_RAM_isrodata_kernel },
	{ __start_RAM_stext_kernel_A27, __end_RAM_stext_kernel_A27, __start_RAM_istext_kernel_A27 },
	{ __start_RAM_srodata_kernel_A23, __end_RAM_srodata_kernel_A23, __start_RAM_isrodata_kernel_A23 }
};

/*
 *  BSS Section Initialization Information
 */

extern char __start_bss_kernel[];
extern char __end_bss_kernel[];

const uint_t _kernel_tnum_bsssec = 1U;

const BSSSECINIB _kernel_bsssecinib_table[1] = {
	{ __start_bss_kernel, __end_bss_kernel }
};

