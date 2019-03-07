/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2005-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_svc.c 613 2018-12-19 23:38:01Z ertl-hiro $
 */

/* 
 *		テストプログラム用サービス（非TECS版専用）
 */

#include <kernel.h>
#include <sil.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/syslog.h"
#include "target_syssvc.h"
#include "test_svc.h"

/*
 *	チェックポイント
 */
static uint_t	check_count;

/*
 *  テストプログラムの開始
 */
void
_test_start(const char *progname)
{
	syslog_1(LOG_NOTICE, "Test program: %s", progname);
	check_count = 0U;
}

/*
 *	テストプログラムの終了
 */
static void
test_finish(void)
{
	SIL_PRE_LOC;

	SIL_LOC_INT();
	(void) syslog_fls_log();
	(void) ext_ker();

	/* ここへ来ることはないはず */
	SIL_UNL_INT();
}

/*
 *	チェックポイント
 */
void
_check_point(uint_t count)
{
	bool_t		errorflag = false;
#ifdef CHECK_BIT_FUNC
	ER			rercd;
	extern ER	CHECK_BIT_FUNC(void);
#endif /* CHECK_BIT_FUNC */
	SIL_PRE_LOC;

	/*
	 *  割込みロック状態に
	 */
	SIL_LOC_INT();

	/*
	 *  シーケンスチェック
	 */
	if (++check_count == count) {
		syslog_1(LOG_NOTICE, "Check point %d passed.", count);
	}
	else {
		syslog_1(LOG_ERROR, "## Unexpected check point %d.\007", count);
		errorflag = true;
	}

	/*
	 *  カーネルの内部状態の検査
	 */
#ifdef CHECK_BIT_FUNC
	rercd = CHECK_BIT_FUNC();
	if (rercd < 0) {
		syslog_2(LOG_ERROR, "## Internal inconsistency detected (%s, %d).\007",
										itron_strerror(rercd), SERCD(rercd));
		errorflag = true;
	}
#endif /* CHECK_BIT_FUNC */

	/*
	 *  エラーが検出された場合は，テストプログラムを終了する．
	 */
	if (errorflag) {
		test_finish();
	}

	/*
	 *  割込みロック状態を解除
	 */
	SIL_UNL_INT();
}

/*
 *	完了チェックポイント
 */
void
_check_finish(uint_t count)
{
	if (count > 0U) {
		_check_point(count);
		syslog_0(LOG_NOTICE, "All check points passed.");
	}
	test_finish();
}

/*
 *	条件チェックのエラー処理
 */
void
_check_assert_error(const char *expr, const char *file, int_t line)
{
	syslog_3(LOG_ERROR, "## Assertion `%s' failed at %s:%u.\007",
												expr, file, line);
	test_finish();
}

/*
 *	エラーコードチェックのエラー処理
 */
void
_check_ercd_error(ER ercd, const char *file, int_t line)
{
	syslog_3(LOG_ERROR, "## Unexpected error %s detected at %s:%u.\007",
									itron_strerror(ercd), file, line);
	test_finish();
}

/*
 *	割込み優先度マスクのチェック
 */
ER
_get_interrupt_priority_mask(PRI *p_intpri)
{
	return(get_ipm(p_intpri));
}

/*
 *  テストプログラム用サービスのサービスコールを拡張サービスコールとし
 *  て登録するための定義
 */
ER_UINT
extsvc_test_start(intptr_t progname, intptr_t par2, intptr_t par3,
							intptr_t par4, intptr_t par5, ID cdmid)
{
	_test_start((char *) progname);
	return(E_OK);
}

ER_UINT
extsvc_check_point(intptr_t count, intptr_t par2, intptr_t par3,
							intptr_t par4, intptr_t par5, ID cdmid)
{
	_check_point((uint_t) count);
	return(E_OK);
}

ER_UINT
extsvc_check_finish(intptr_t count, intptr_t par2, intptr_t par3,
							intptr_t par4, intptr_t par5, ID cdmid)
{
	_check_finish((uint_t) count);
	return(E_OK);
}

ER_UINT
extsvc_check_assert_error(intptr_t expr, intptr_t file, intptr_t line,
							intptr_t par4, intptr_t par5, ID cdmid)
{
	_check_assert_error((const char *) expr,
								(const char *) file, (int_t) line);
	return(E_OK);
}

ER_UINT
extsvc_check_ercd_error(intptr_t ercd, intptr_t file, intptr_t line,
							intptr_t par4, intptr_t par5, ID cdmid)
{
	_check_ercd_error((ER) ercd, (const char *) file, (int_t) line);
	return(E_OK);
}

ER_UINT
extsvc_get_interrupt_priority_mask(intptr_t p_intpri, intptr_t par2,
					 intptr_t par3, intptr_t par4, intptr_t par5, ID cdmid)
{
	if (!EXTSVC_PROBE_MEM_WRITE((PRI *) p_intpri, PRI)) {
		return(E_MACV);
	}
	return(_get_interrupt_priority_mask((PRI *) p_intpri));
}
