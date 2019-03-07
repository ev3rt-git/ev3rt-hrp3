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
#include "cfg_attmod1.h"

/*
 *  Memory Object Initialization Information
 */

extern char __start_mo_text_DOM2[];
extern char __start_mo_rodata_DOM2[];
extern char __start_mo_text_shared[];
extern char __start_mo_rodata_shared[];
extern char __aend_mo_rodata_shared[];
extern char __start_ustack_TASK2[];
extern char __start_ustack_TOPPERS_RAM_DUMMY1[];
extern char __start_ustack_TASK2_0[];
extern char __start_mo_rwdata_DOM2[];
extern char __start_mo_rwdata_shared[];
extern char __aend_mo_rwdata_shared[];
extern char __start_mo_ROM2_text_DOM1[];
extern char __start_mo_ROM2_rodata_DOM1[];
extern char __aend_mo_ROM2_rodata_DOM1[];
extern char __start_ustack_TASK1[];
extern char __start_ustack_TOPPERS_RAM2_DUMMY1[];
extern char __start_ustack_TASK1_0[];
extern char __start_mo_RAM2_rwdata_DOM1[];
extern char __aend_mo_RAM2_rwdata_DOM1[];
extern char __start_mo_ROM3_text_kernel[];
extern char __start_mo_ROM3_rodata_kernel[];
extern char __aend_mo_ROM3_rodata_kernel[];
extern char __start_mo_RAM3_rwdata_kernel[];
extern char __aend_mo_RAM3_rwdata_kernel[];

const uint_t _kernel_tnum_meminib = 25U;

void *const _kernel_memtop_table[25] = {
	0,
	__start_mo_text_DOM2 /* 0x01000000 */,
	__start_mo_rodata_DOM2 /* 0x01001000 */,
	__start_mo_text_shared /* 0x01002000 */,
	__start_mo_rodata_shared /* 0x01003000 */,
	__aend_mo_rodata_shared /* 0x01004000 */,
	__start_ustack_TASK2 /* 0x01800000 */,
	__start_ustack_TOPPERS_RAM_DUMMY1 /* 0x01801000 */,
	__start_ustack_TASK2_0 /* 0x01802000 */,
	__start_mo_rwdata_DOM2 /* 0x01803000 */,
	__start_mo_rwdata_shared /* 0x01804000 */,
	__aend_mo_rwdata_shared /* 0x01805000 */,
	__start_mo_ROM2_text_DOM1 /* 0x02000000 */,
	__start_mo_ROM2_rodata_DOM1 /* 0x02001000 */,
	__aend_mo_ROM2_rodata_DOM1 /* 0x02002000 */,
	__start_ustack_TASK1 /* 0x02800000 */,
	__start_ustack_TOPPERS_RAM2_DUMMY1 /* 0x02801000 */,
	__start_ustack_TASK1_0 /* 0x02802000 */,
	__start_mo_RAM2_rwdata_DOM1 /* 0x02803000 */,
	__aend_mo_RAM2_rwdata_DOM1 /* 0x02804000 */,
	__start_mo_ROM3_text_kernel /* 0x03000000 */,
	__start_mo_ROM3_rodata_kernel /* 0x0300a000 */,
	__aend_mo_ROM3_rodata_kernel /* 0x0300b000 */,
	__start_mo_RAM3_rwdata_kernel /* 0x03800000 */,
	__aend_mo_RAM3_rwdata_kernel /* 0x03806000 */
};

const MEMINIB _kernel_meminib_table[25] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP(DOM2), TACP(DOM2) },
	{ TA_RODATASEC, 0U, TACP(DOM2), TACP(DOM2) },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_RODATASEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM2), TACP(DOM2), TACP(DOM2) },
	{ TA_USTACK|(TA_NOINITSEC), TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM2), TACP(DOM2), TACP(DOM2) },
	{ 0x0, TACP(DOM2), TACP(DOM2), TACP(DOM2) },
	{ 0x0, TACP_SHARED, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP(DOM1), TACP(DOM1) },
	{ TA_RODATASEC, 0U, TACP(DOM1), TACP(DOM1) },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ TA_USTACK|(TA_NOINITSEC), TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ 0x0, TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U }
};

/*
 *  Data Section Initialization Information
 */

extern char __start_data_DOM2[];
extern char __end_data_DOM2[];
extern char __start_idata_DOM2[];
extern char __start_data_shared[];
extern char __end_data_shared[];
extern char __start_idata_shared[];
extern char __start_RAM2_data_DOM1[];
extern char __end_RAM2_data_DOM1[];
extern char __start_RAM2_idata_DOM1[];
extern char __start_RAM3_data_kernel[];
extern char __end_RAM3_data_kernel[];
extern char __start_RAM3_idata_kernel[];

const uint_t _kernel_tnum_datasec = 4U;

const DATASECINIB _kernel_datasecinib_table[4] = {
	{ __start_data_DOM2, __end_data_DOM2, __start_idata_DOM2 },
	{ __start_data_shared, __end_data_shared, __start_idata_shared },
	{ __start_RAM2_data_DOM1, __end_RAM2_data_DOM1, __start_RAM2_idata_DOM1 },
	{ __start_RAM3_data_kernel, __end_RAM3_data_kernel, __start_RAM3_idata_kernel }
};

/*
 *  BSS Section Initialization Information
 */

extern char __start_bss_DOM2[];
extern char __end_bss_DOM2[];
extern char __start_bss_shared[];
extern char __end_bss_shared[];
extern char __start_RAM2_bss_DOM1[];
extern char __end_RAM2_bss_DOM1[];
extern char __start_RAM3_bss_kernel[];
extern char __end_RAM3_bss_kernel[];

const uint_t _kernel_tnum_bsssec = 4U;

const BSSSECINIB _kernel_bsssecinib_table[4] = {
	{ __start_bss_DOM2, __end_bss_DOM2 },
	{ __start_bss_shared, __end_bss_shared },
	{ __start_RAM2_bss_DOM1, __end_RAM2_bss_DOM1 },
	{ __start_RAM3_bss_kernel, __end_RAM3_bss_kernel }
};

