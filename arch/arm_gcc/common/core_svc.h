/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2018 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: core_svc.h 592 2018-12-08 01:59:36Z ertl-hiro $
 */

/*
 *	ソフトウェア割込みによるサービスコール呼出し（ARM用）
 */

#ifndef TOPPERS_CORE_SVC_H
#define TOPPERS_CORE_SVC_H

/*
 *  svc命令を用いたサービスコールの呼び出し方法
 */

#define	SVC_SERVICE_CALL	1

#ifndef TOPPERS_MACRO_ONLY

#define CAL_SVC_0M(TYPE, FNCD) do {								\
	register TYPE r0 asm("r0");									\
	register FN r5 asm("r5") = FNCD;							\
	Asm("svc %1"												\
	  : "=r"(r0)												\
	  : "I"(SVC_SERVICE_CALL),"r"(r5)							\
	  : "r1","r2","r3","r12","lr","memory","cc");				\
	return((TYPE) r0);											\
} while (false)

#define CAL_SVC_1M(TYPE, FNCD, TYPE1, PAR1) do {				\
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1);				\
	register FN r5 asm("r5") = FNCD;							\
	Asm("svc %1"												\
	  : "=r"(r0)												\
	  : "I"(SVC_SERVICE_CALL),"0"(r0),"r"(r5)					\
	  : "r1","r2","r3","r12","lr","memory","cc");				\
	return((TYPE) r0);											\
} while (false)

#define CAL_SVC_2M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2) do {	\
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1);				\
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2);				\
	register FN r5 asm("r5") = FNCD;							\
	Asm("svc %2"												\
	  : "=r"(r0),"=r"(r1)										\
	  : "I"(SVC_SERVICE_CALL),"0"(r0),"1"(r1),"r"(r5)			\
	  : "r2","r3","r12","lr","memory","cc");					\
	return((TYPE) r0);											\
} while (false)

#define CAL_SVC_3M(TYPE, FNCD, TYPE1, PAR1,						\
							TYPE2, PAR2, TYPE3, PAR3) do {		\
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1);				\
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2);				\
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3);				\
	register FN r5 asm("r5") = FNCD;							\
	Asm("svc %3"												\
	  : "=r"(r0),"=r"(r1),"=r"(r2)								\
	  : "I"(SVC_SERVICE_CALL),"0"(r0),"1"(r1),"2"(r2),			\
		"r"(r5)													\
	  : "r3","r12","lr","memory","cc");							\
	return((TYPE) r0);											\
} while (false)

#define CAL_SVC_4M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2,		\
								TYPE3, PAR3, TYPE4, PAR4) do {	\
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1);				\
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2);				\
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3);				\
	register TYPE4 r3 asm("r3") = (TYPE4)(PAR4); 				\
	register FN r5 asm("r5") = FNCD;							\
	Asm("svc %4"												\
	  : "=r"(r0),"=r"(r1),"=r"(r2),"=r"(r3)						\
	  : "I"(SVC_SERVICE_CALL),"0"(r0),"1"(r1),"2"(r2),			\
		"3"(r3),"r"(r5)											\
	  : "r12","lr","memory","cc");								\
	return((TYPE) r0);											\
} while (false)

#define CAL_SVC_5M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2,		\
					TYPE3, PAR3, TYPE4, PAR4, TYPE5, PAR5) do {	\
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1);				\
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2);				\
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3);				\
	register TYPE4 r3 asm("r3") = (TYPE4)(PAR4); 				\
	register TYPE5 r4 asm("r4") = (TYPE5)(PAR5);				\
	register FN r5 asm("r5") = FNCD;							\
	Asm("svc %5"												\
	  : "=r"(r0),"=r"(r1),"=r"(r2),"=r"(r3),"=r"(r4)			\
	  : "I"(SVC_SERVICE_CALL),"0"(r0),"1"(r1),"2"(r2),			\
		"3"(r3),"r"(r4),"r"(r5)									\
	  : "r12","lr","memory","cc");								\
	return((TYPE) r0);											\
} while (false)

#define CAL_SVC_1M_SYSTIM(TYPE, FNCD, TYPE1, PAR1) do {					\
	register intptr_t r0 asm("r0") = (intptr_t)((PAR1) & 0xffffffffU); 	\
	register intptr_t r1 asm("r1") = (intptr_t)((PAR1) >> 32);			\
	register FN r5 asm("r5") = FNCD;									\
	Asm("svc %2"														\
	  : "=r"(r0),"=r"(r1)												\
	  : "I"(SVC_SERVICE_CALL),"0"(r0),"1"(r1),"r"(r5)					\
	  : "r2","r3","r12","lr","memory","cc");							\
	return((TYPE) r0);													\
} while (false)

/*
 *  サービスコール呼出し関数の展開
 */
#ifndef TOPPERS_SVC_CALL
#include "arch/gcc/tool_svc.h"
#endif /* TOPPERS_SVC_CALL */

/*
 *  cal_svcサービスコールの呼出し
 */
Inline ER_UINT
cal_svc(FN fncd, intptr_t par1, intptr_t par2,
							intptr_t par3, intptr_t par4, intptr_t par5)
{
	if (fncd > 0) {
		CAL_SVC_5M(ER_UINT, fncd, intptr_t, par1, intptr_t, par2,
							intptr_t, par3, intptr_t, par4, intptr_t, par5);
	}
	else {
		return(E_RSFN);
	}
}

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CORE_SVC_H */
