/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2006-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: histogram.h 415 2018-07-27 09:06:40Z ertl-hiro $
 */

/*
 *		実行時間分布集計サービス
 */

#ifndef TOPPERS_HISTOGRAM_H
#define TOPPERS_HISTOGRAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <t_stddef.h>

#ifdef TOPPERS_OMIT_TECS

/*
 *  実行時間分布集計サービスの拡張サービスコールのスタックサイズ
 */ 
#ifndef SSZ_INIT_HIST
#define SSZ_INIT_HIST		1024
#endif /* SSZ_INIT_HIST */

#ifndef SSZ_BEGIN_MEASURE
#define SSZ_BEGIN_MEASURE	1024
#endif /* SSZ_BEGIN_MEASURE */

#ifndef SSZ_END_MEASURE
#define SSZ_END_MEASURE		1024
#endif /* SSZ_END_MEASURE */

#ifndef SSZ_PRINT_HIST
#define SSZ_PRINT_HIST		1024
#endif /* SSZ_PRINT_HIST */

#endif /* TOPPERS_OMIT_TECS */

/*
 *  実行時間分布集計サービスのサービスコール
 */
#ifndef TOPPERS_OMIT_TECS

extern ER	init_hist(ID histid);
extern ER	begin_measure(ID histid);
extern ER	end_measure(ID histid);
extern ER	print_hist(ID histid);

#else /* TOPPERS_OMIT_TECS */

extern ER	_init_hist(ID histid);
extern ER	_begin_measure(ID histid);
extern ER	_end_measure(ID histid);
extern ER	_print_hist(ID histid);

/*
 *  実行時間分布集計サービスのサービスコールを呼び出すための定義
 */
#ifndef TOPPERS_SVC_CALL

Inline ER
init_hist(ID histid)
{
	return((ER) cal_svc(TFN_INIT_HIST, (intptr_t) histid, 0, 0, 0, 0));
}

Inline ER
begin_measure(ID histid)
{
	return((ER) cal_svc(TFN_BEGIN_MEASURE, (intptr_t) histid, 0, 0, 0, 0));
}

Inline ER
end_measure(ID histid)
{
	return((ER) cal_svc(TFN_END_MEASURE, (intptr_t) histid, 0, 0, 0, 0));
}

Inline ER
print_hist(ID histid)
{
	return((ER) cal_svc(TFN_PRINT_HIST, (intptr_t) histid, 0, 0, 0, 0));
}

#else /* TOPPERS_SVC_CALL */

#define init_hist			_init_hist
#define begin_measure		_begin_measure
#define end_measure			_end_measure
#define print_hist			_print_hist

#endif /* TOPPERS_SVC_CALL */

/*
 *  実行時間分布集計サービスのサービスコールを拡張サービスコールとして
 *  登録するための関数
 */
extern ER_UINT	extsvc_init_hist(intptr_t histid, intptr_t par2,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();
extern ER_UINT	extsvc_begin_measure(intptr_t histid, intptr_t par2,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();
extern ER_UINT	extsvc_end_measure(intptr_t histid, intptr_t par2,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();
extern ER_UINT	extsvc_print_hist(intptr_t histid, intptr_t par2,
									intptr_t par3, intptr_t par4,
									intptr_t par5, ID cdmid) throw();

#endif /* TOPPERS_OMIT_TECS */

#ifdef __cplusplus
}
#endif

#endif /* TOPPERS_HISTOGRAM_H */
