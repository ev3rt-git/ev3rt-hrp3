/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2006-2016 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: core_test.h 94 2016-01-16 05:23:36Z ertl-hiro $
 */

/*
 *		テストプログラムのコア依存部（ARM用）
 *
 *  このヘッダファイルは，target_test.h（または，そこからインクルードさ
 *  れるファイル）のみからインクルードされる．他のファイルから直接イン
 *  クルードしてはならない．
 */

#ifndef TOPPERS_CORE_TEST_H
#define TOPPERS_CORE_TEST_H

#include <t_stddef.h>
#include "arm.h"

/*
 *  メモリアクセス違反ハンドラのテストのための定義
 */
#define EXCNO_MACV_INST			EXCNO_PABORT
#define EXCNO_MACV_DATA			EXCNO_DABORT

#define PREPARE_RETURN_MACV_INST
#define PREPARE_RETURN_MACV_DATA	((T_EXCINF *) p_excinf)->pc -= 4U

/*
 *  CPU例外の発生
 */

#ifndef ILLEGAL_IADDR
#define ILLEGAL_IADDR			0xd0000000U		/* 不正命令アドレス */
#endif /* ILLEGAL_IADDR */

#ifndef ILLEGAL_DADDR
#define ILLEGAL_DADDR			0xd0000000U		/* 不正データアドレス */
#endif /* ILLEGAL_DADDR */

/*
 *  スーパバイザコールによるCPU例外の発生
 *
 *  CPU例外ハンドラから，そのままリターンすることができる．svc命令によ
 *  りlrレジスタが上書きされるため，lrレジスタは破壊されるものと指定し
 *  ている．
 */
#if defined(USE_SVC_CPUEXC)

#define CPUEXC1					EXCNO_SVC		/* スーパバイザコール */
#define RAISE_CPU_EXCEPTION		Asm("svc #0":::"lr")
#define PREPARE_RETURN_CPUEXC

/*
 *  プリフェッチアボートによるCPU例外の発生
 *
 *  プリフェッチアボートを起こした命令をスキップしてCPU例外ハンドラから
 *  リターンするために，戻り番地はそのままで良い（ARMモードで使うことを
 *  想定）．
 */
#elif defined(USE_PABORT_CPUEXC)

#define CPUEXC1					EXCNO_PABORT	/* プリフェッチアボート */
#define RAISE_CPU_EXCEPTION		((void (*)(void)) ILLEGAL_IADDR)()
#define PREPARE_RETURN_CPUEXC

/*
 *  データアボートによるCPU例外の発生
 *
 *  データアボートを起こした命令をスキップしてCPU例外ハンドラからリター
 *  ンするために，戻り番地から4を減算する（ARMモードで使うことを想定）．
 */
#elif defined(USE_DABORT_CPUEXC)

#include "arm.h"
#define CPUEXC1					EXCNO_DABORT	/* データアボート */
#define RAISE_CPU_EXCEPTION		(void)(*((volatile uint32_t *) ILLEGAL_DADDR))
#define PREPARE_RETURN_CPUEXC	((T_EXCINF *) p_excinf)->pc -= 4U

/*
 *  フェイタルデータアボート処理
 *
 *  スタックポインタを不正にして，フェイタルデータアボート処理を行わせ
 *  る．CPU例外ハンドラからはリターンできない．
 */
#elif defined(USE_FATAL_DABORT_CPUEXC)

#define CPUEXC1					EXCNO_DABORT	/* データアボート */
#define RAISE_CPU_EXCEPTION		Asm("mov sp, %0"::"I"(ILLEGAL_DADDR))

/*
 *  未定義命令によるCPU例外の発生
 *
 *  RAISE_CPU_EXCEPTIONは，ARMモードで使うことを想定している．CPU例外ハ
 *  ンドラから，そのままリターンすることができる．使用している未定義命
 *  令は，"Multiply and multiply accumulate"命令群のエンコーディング内
 *  における未定義命令である．
 */
#else

#define CPUEXC1					EXCNO_UNDEF		/* 未定義命令 */
#define RAISE_CPU_EXCEPTION		Asm(".word 0xf0500090")
#define PREPARE_RETURN_CPUEXC

#endif
#endif /* TOPPERS_CORE_TEST_H */
