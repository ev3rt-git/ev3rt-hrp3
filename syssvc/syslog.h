/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  $Id: syslog.h 415 2018-07-27 09:06:40Z ertl-hiro $
 */

/*
 *		システムログ機能
 */

#ifndef TOPPERS_SYSLOG_H
#define TOPPERS_SYSLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <t_stddef.h>
#include <t_syslog.h>

#ifdef TOPPERS_OMIT_TECS
#include <extsvc_fncode.h>
#include "target_syssvc.h"

/*
 *  システムログ機能の拡張サービスコールのスタックサイズ
 */ 
#ifndef SSZ_SYSLOG_WRI_LOG
#define SSZ_SYSLOG_WRI_LOG	1024
#endif /* SSZ_SYSLOG_WRI_LOG */

#ifndef SSZ_SYSLOG_REA_LOG
#define SSZ_SYSLOG_REA_LOG	1024
#endif /* SSZ_SYSLOG_REA_LOG */

#ifndef SSZ_SYSLOG_MSK_LOG
#define SSZ_SYSLOG_MSK_LOG	1024
#endif /* SSZ_SYSLOG_MSK_LOG */

#ifndef SSZ_SYSLOG_REF_LOG
#define SSZ_SYSLOG_REF_LOG	1024
#endif /* SSZ_SYSLOG_REF_LOG */

#ifndef SSZ_SYSLOG_FLS_LOG
#define SSZ_SYSLOG_FLS_LOG	1024
#endif /* SSZ_SYSLOG_FLS_LOG */

#endif /* TOPPERS_OMIT_TECS */

/*
 *  ログ情報の重要度のビットマップを作るためのマクロ
 */
#define LOG_MASK(prio)		(1U << (prio))
#define LOG_UPTO(prio)		((1U << ((prio) + 1)) - 1)

/*
 *  パケット形式の定義
 */
typedef struct t_syslog_rlog {
	uint_t	count;		/* ログバッファ中のログの数 */
	uint_t	lost;		/* 失われたログの数 */
	uint_t	logmask;	/* ログバッファに記録すべき重要度 */
	uint_t	lowmask;	/* 低レベル出力すべき重要度 */
} T_SYSLOG_RLOG;

/*
 *  システムログ機能のサービスコール
 */
#ifndef TOPPERS_OMIT_TECS

extern ER		syslog_wri_log(uint_t prio, const SYSLOG *p_syslog) throw();
extern ER_UINT	syslog_rea_log(SYSLOG *p_syslog) throw();
extern ER		syslog_msk_log(uint_t logmask, uint_t lowmask) throw();
extern ER		syslog_ref_log(T_SYSLOG_RLOG *pk_rlog) throw();
extern ER		syslog_fls_log(void) throw();

#else /* TOPPERS_OMIT_TECS */

extern ER		_syslog_wri_log(uint_t prio, const SYSLOG *p_syslog) throw();
extern ER_UINT	_syslog_rea_log(SYSLOG *p_syslog) throw();
extern ER		_syslog_msk_log(uint_t logmask, uint_t lowmask) throw();
extern ER		_syslog_ref_log(T_SYSLOG_RLOG *pk_rlog) throw();
extern ER		_syslog_fls_log(void) throw();

/*
 *  システムログ機能の初期化
 */
extern void	syslog_initialize(intptr_t exinf) throw();

/*
 *  システムログ機能のサービスコールを呼び出すための定義
 */
#ifndef TOPPERS_SVC_CALL

Inline ER_UINT
syslog_rea_log(SYSLOG *p_syslog)
{
	return(cal_svc(TFN_SYSLOG_REA_LOG, (intptr_t) p_syslog, 0, 0, 0, 0));
}

Inline ER
syslog_msk_log(intptr_t logmask, intptr_t lowmask)
{
	return((ER) cal_svc(TFN_SYSLOG_MSK_LOG, (intptr_t) logmask,
											(intptr_t) lowmask, 0, 0, 0));
}

Inline ER
syslog_ref_log(intptr_t pk_rlog)
{
	return((ER) cal_svc(TFN_SYSLOG_REF_LOG, (intptr_t) pk_rlog, 0, 0, 0, 0));
}

Inline ER
syslog_fls_log(void)
{
	return((ER) cal_svc(TFN_SYSLOG_FLS_LOG, 0, 0, 0, 0, 0));
}

#else /* TOPPERS_SVC_CALL */

#define syslog_rea_log		_syslog_rea_log
#define syslog_msk_log		_syslog_msk_log
#define syslog_ref_log		_syslog_ref_log
#define syslog_fls_log		_syslog_fls_log

#endif /* TOPPERS_SVC_CALL */

/*
 *  システムログ機能のサービスコールを拡張サービスコールとして登録する
 *  ための関数
 */
extern ER_UINT	extsvc_syslog_wri_log(intptr_t prio, intptr_t p_syslog,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();
extern ER_UINT	extsvc_syslog_rea_log(intptr_t p_syslog, intptr_t par2,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();
extern ER_UINT	extsvc_syslog_msk_log(intptr_t logmask, intptr_t lowmask,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();
extern ER_UINT	extsvc_syslog_ref_log(intptr_t pk_rlog, intptr_t par2,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();
extern ER_UINT	extsvc_syslog_fls_log(intptr_t par1, intptr_t par2,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();

#endif /* TOPPERS_OMIT_TECS */

#ifdef __cplusplus
}
#endif

#endif /* TOPPERS_SYSLOG_H */
