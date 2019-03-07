/*
 *		cfg1_out.cのリンクに必要なスタブの定義
 *
 *  $Id: core_cfg1_out.h 325 2018-04-07 09:48:56Z ertl-hiro $
 */

#include <kernel.h>

void sta_ker(void) { }
void _kernel_initialize_sections(void) { }
STK_T *const	_kernel_istkpt;
