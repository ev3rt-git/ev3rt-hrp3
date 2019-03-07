/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  $Id: core_kernel_impl.h 663 2019-02-03 11:48:59Z ertl-hiro $
 */

/*
 *		kernel_impl.hのコア依存部（ARM用）
 *
 *  このヘッダファイルは，target_kernel_impl.h（または，そこからインク
 *  ルードされるファイル）のみからインクルードされる．他のファイルから
 *  直接インクルードしてはならない．
 */

#ifndef TOPPERS_CORE_KERNEL_IMPL_H
#define TOPPERS_CORE_KERNEL_IMPL_H

#include "arm.h"

/*
 *  ターゲット依存のタスク属性（エラーチェック用）
 */
#ifdef USE_ARM_FPU
#define TARGET_TSKATR		(TA_FPU)
#endif /* USE_ARM_FPU */

/*
 *  ターゲット依存のメモリオブジェクト属性（エラーチェック用）
 */
#define TARGET_MEMATR		(TA_SORDER|TA_WTHROUGH|TA_NONSHARED)

/*
 *  ターゲット依存のメモリオブジェクトアクセス属性（エラーチェック用）
 */
#define TARGET_ACCATR		(TA_SORDER|TA_WTHROUGH|TA_NONSHARED)

/*
 *  ターゲット依存のメモリオブジェクト属性
 */
#define TARGET_MEMATR_SSTACK	(TA_NOINITSEC|TA_NONSHARED)
#define TARGET_MEMATR_USTACK	(TA_NOINITSEC|TA_NONSHARED)

/*
 *  エラーチェック方法の指定
 */
#define CHECK_SSTKSZ_ALIGN	8		/* システムスタックサイズのアライン単位 */
#define CHECK_USTKSZ_ALIGN	4096	/* ユーザスタックサイズのアライン単位 */
#define CHECK_INTPTR_ALIGN	4		/* intptr_t型の変数のアライン単位 */
#define CHECK_INTPTR_NONNULL		/* intptr_t型の変数の非NULLチェック */
#ifndef  __thumb__
#define CHECK_FUNC_ALIGN	4		/* 関数のアライン単位 */
#endif /* __thumb__ */
#define CHECK_FUNC_NONNULL			/* 関数の非NULLチェック */
#define CHECK_SSTACK_ALIGN	8		/* システムスタック領域のアライン単位 */
#define CHECK_SSTACK_NONNULL		/* システムスタック領域の非NULLチェック */
#define CHECK_USTACK_ALIGN	4096	/* ユーザスタック領域のアライン単位 */
#define CHECK_USTACK_NONNULL		/* ユーザスタック領域の非NULLチェック */
#define CHECK_MPF_ALIGN		4		/* 固定長メモリプール領域のアライン単位 */
#define CHECK_MPF_NONNULL			/* 固定長メモリプール領域の非NULLチェック */
#define CHECK_MPK_ALIGN		4		/* カーネルメモリプール領域のアライン単位 */
#define CHECK_MPK_NONNULL			/* カーネルメモリプール領域の非NULL  */
									/*							チェック */
#define CHECK_MB_ALIGN		4		/* 管理領域のアライン単位 */

/*
 *  デフォルトのスタックサイズ
 */
#ifndef DEFAULT_SSTKSZ
#define DEFAULT_SSTKSZ		2048	/* タスクのシステムスタックのサイズ */
#endif /* DEFAULT_SSTKSZ */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  レディキューサーチのためのビットマップサーチ関数
 *
 *  CLZ命令は最上位ビットからサーチするため，最上位ビットを最高優先度に
 *  対応させる．
 */
#if __TARGET_ARCH_ARM >= 6

#define	OMIT_BITMAP_SEARCH
#define	PRIMAP_BIT(pri)		(0x8000U >> (pri))

Inline uint_t
bitmap_search(uint16_t bitmap)
{
	return((uint_t)(count_leading_zero((uint32_t) bitmap) - 16));
}

#endif /* __TARGET_ARCH_ARM >= 6 */

/*
 *  コンテキストの参照
 *
 *  ARM依存部では，タスクコンテキストと非タスクコンテキストの両方をスー
 *  パバイザモードで動作させるため，プロセッサモードで判断することがで
 *  きない．そのため，割込みハンドラ／CPU例外ハンドラのネスト段数（これ
 *  を，例外ネストカウントと呼ぶ）で管理し，例外ネストカウントが0の時に
 *  タスクコンテキスト，0より大きい場合に非タスクコンテキストであると判
 *  断する．
 */
extern uint32_t	excpt_nest_count;		/* 例外ネストカウント */

Inline bool_t
sense_context(void)
{
	return(excpt_nest_count > 0U);
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  TOPPERS標準割込み処理モデルの実現
 *
 *  ARMコア依存部では，割込みの扱いに関して，次の2つの方法をサポートす
 *  る．
 *
 *  (1) カーネルを単体で使用する場合やSafeGのノンセキュアモードで使用す
 *  る場合：IRQをカーネル管理の割込み，FIQをカーネル管理外の割込みとす
 *  る．デフォルトでは，この方法が使用される．
 *
 *  (2) SafeGのセキュアモードで使用する場合：FIQをカーネル管理の割込み
 *  とし，カーネルの動作中はIRQを常にマスクする．この方法を使用する場合
 *  には，TOPPERS_SAFEG_SECUREをマクロ定義する．
 * 
 *  TOPPERS標準割込み処理モデルの中で，割込み優先度マスクと割込み要求禁
 *  止フラグに関しては，割込みコントローラによって実現方法が異なるため，
 *  ARMコア依存部では扱わない．
 */

/*
 *  FIQとIRQの両方を禁止するCPSRのビットパターン
 */
#ifndef TOPPERS_MACRO_ONLY
#define CPSR_FIQ_IRQ_BIT	(CPSR_FIQ_BIT|CPSR_IRQ_BIT)
#else /* TOPPERS_MACRO_ONLY */
#define CPSR_FIQ_IRQ_BIT	(CPSR_FIQ_BIT AOR CPSR_IRQ_BIT)
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  CPUロック・割込みロックでない状態でのCPSRのビットパターン
 */
#ifndef TOPPERS_SAFEG_SECURE
#define CPSR_UNLOCK			UINT_C(0x00)
#else /* TOPPERS_SAFEG_SECURE */
#define CPSR_UNLOCK			CPSR_IRQ_BIT
#endif /* TOPPERS_SAFEG_SECURE */

/*
 *  CPUロック状態でのCPSRのビットパターン
 */
#ifndef TOPPERS_SAFEG_SECURE
#define CPSR_CPULOCK		CPSR_IRQ_BIT
#else /* TOPPERS_SAFEG_SECURE */
#define CPSR_CPULOCK		CPSR_FIQ_IRQ_BIT
#endif /* TOPPERS_SAFEG_SECURE */

/*
 *  割込みロック状態でのCPSRのビットパターン
 */
#define CPSR_INTLOCK		CPSR_FIQ_IRQ_BIT

#ifndef TOPPERS_MACRO_ONLY

/*
 *  CPUロック状態への遷移
 */
Inline void
lock_cpu(void)
{
#if __TARGET_ARCH_ARM < 6
	set_cpsr(current_cpsr() | CPSR_CPULOCK);
#else /* __TARGET_ARCH_ARM < 6 */
#ifndef TOPPERS_SAFEG_SECURE
	disable_irq();
#else /* TOPPERS_SAFEG_SECURE */
	disable_fiq()
#endif /* TOPPERS_SAFEG_SECURE */
#endif /* __TARGET_ARCH_ARM < 6 */

	/*
	 *  メモリ参照が，この関数を超えて最適化されることを抑止
	 */
	ARM_MEMORY_CHANGED;
}

/*
 *  CPUロック状態への移行（ディスパッチできる状態）
 */
#define lock_cpu_dsp()		lock_cpu()

/*
 *  CPUロック状態の解除
 */
Inline void
unlock_cpu(void)
{
	/*
	 *  メモリ参照が，この関数を超えて最適化されることを抑止
	 */
	ARM_MEMORY_CHANGED;

#if __TARGET_ARCH_ARM < 6
	set_cpsr((current_cpsr() & ~CPSR_INT_MASK) | CPSR_UNLOCK);
#else /* __TARGET_ARCH_ARM < 6 */
#ifndef TOPPERS_SAFEG_SECURE
	enable_irq();
#else /* TOPPERS_SAFEG_SECURE */
	enable_fiq()
#endif /* TOPPERS_SAFEG_SECURE */
#endif /* __TARGET_ARCH_ARM < 6 */
}

/*
 *  CPUロック状態の解除（ディスパッチできる状態）
 */
#define unlock_cpu_dsp()	unlock_cpu()

/*
 *  CPUロック状態の参照
 */
Inline bool_t
sense_lock(void)
{
#ifndef TOPPERS_SAFEG_SECURE
	return((current_cpsr() & CPSR_IRQ_BIT) != 0U);
#else /* TOPPERS_SAFEG_SECURE */
	return((current_cpsr() & CPSR_FIQ_BIT) != 0U);
#endif /* TOPPERS_SAFEG_SECURE */
}

/*
 *  割込みを受け付けるための遅延処理
 */
Inline void
delay_for_interrupt(void)
{
}

/* 
 *  保護ドメイン初期化コンテキストブロックを使う
 */
#define USE_DOMINICTXB

/*
 *  保護ドメイン初期化コンテキストブロックの定義
 */
typedef	struct domain_initialization_context_block {
	const uint32_t	*p_section_table;	/* セクションテーブルのアドレス */
} DOMINICTXB;

/*
 *  非タスクコンテキスト用のスタック初期値
 */
#define TOPPERS_ISTKPT(istk, istksz)	((STK_T *)((char *)(istk) + (istksz)))

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	void	*sp;		/* スタックポインタ */
	FP		pc;			/* 実行再開番地 */
} TSKCTXB;

/*
 *  アドレス変換テーブルの領域の定義（kernel_mem.c）
 */
extern const uint32_t section_table[][ARM_SECTION_TABLE_ENTRY];
extern const uint32_t page_table[][ARM_PAGE_TABLE_ENTRY];

/*
 *  タスクディスパッチャ
 */

/*
 *  最高優先順位タスクへのディスパッチ（core_support.S）
 *
 *  dispatchは，タスクコンテキストから呼び出されたサービスコール処理か
 *  ら呼び出すべきもので，タスクコンテキスト・CPUロック状態・ディスパッ
 *  チ許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さな
 *  ければならない．
 */
extern void dispatch(void);

/*
 *  非タスクコンテキストからのディスパッチ要求
 */
#define request_dispatch_retint()

/*
 *  ディスパッチャの動作開始（core_support.S）
 *
 *  start_dispatchは，カーネル起動時に呼び出すべきもので，すべての割込
 *  みを禁止した状態（割込みロック状態と同等の状態）で呼び出さなければ
 *  ならない．
 */
extern void start_dispatch(void) NoReturn;

/*
 *  現在のコンテキストを捨ててディスパッチ（core_support.S）
 *
 *  exit_and_dispatchは，ext_tskから呼び出すべきもので，タスクコンテキ
 *  スト・CPUロック状態・ディスパッチ許可状態・（モデル上の）割込み優先
 *  度マスク全解除状態で呼び出さなければならない．
 */
extern void exit_and_dispatch(void) NoReturn;

/*
 *  カーネルの終了処理の呼出し（core_support.S）
 *
 *  call_exit_kernelは，カーネルの終了時に呼び出すべきもので，非タスク
 *  コンテキストに切り換えて，カーネルの終了処理（exit_kernel）を呼び出
 *  す．
 */
extern void call_exit_kernel(void) NoReturn;
     
/*
 *  タスクコンテキストの初期化
 *
 *  タスクが休止状態から実行できる状態に遷移する時に呼ばれる．この時点
 *  でスタック領域を使ってはならない．
 *
 *  activate_contextを，インライン関数ではなくマクロ定義としているのは，
 *  この時点ではTCBが定義されていないためである．
 */
extern void start_stask_r(void);
extern void start_utask_r(void);

#define activate_context(p_tcb)											\
{																		\
	(p_tcb)->tskctxb.sp = (void *)((char *)((p_tcb)->p_tinib->sstk)		\
										+ (p_tcb)->p_tinib->sstksz);	\
	if ((p_tcb)->p_dominib->domptn == TACP_KERNEL) {					\
		(p_tcb)->tskctxb.pc = (FP) start_stask_r;						\
	}																	\
	else {																\
		(p_tcb)->tskctxb.pc = (FP) start_utask_r;						\
	}																	\
}

/*
 *  標準の割込み管理機能の初期化処理を用いない
 */
#define OMIT_INITIALIZE_INTERRUPT

/*
 *  割込みハンドラテーブル（kernel_cfg.c）
 */
extern const FP inh_table[TNUM_INHNO];

/*
 *  割込み要求ライン設定テーブル（kernel_cfg.c）
 * 
 *  割込み要求ラインに対して割込み属性が設定されていれば1，設定されてい
 *  なければ0を保持するテーブル．
 */
#ifdef USE_INTCFG_TABLE
extern const uint8_t intcfg_table[TNUM_INTNO];
#endif /* USE_INTCFG_TABLE */

/*
 *  標準の例外管理機能の初期化処理を用いない
 */
#define OMIT_INITIALIZE_EXCEPTION

/*
 *  CPU例外ハンドラテーブル（kernel_cfg.c）
 */
extern const FP exc_table[TNUM_EXCNO];

/*
 *  CPU例外ハンドラの初期化
 */
Inline void
initialize_exception(void)
{
}

/*
 *  CPU例外の発生した時のコンテキストの参照
 *
 *  CPU例外の発生した時のコンテキストが，タスクコンテキストの時にfalse，
 *  そうでない時にtrueを返す．
 */
Inline bool_t
exc_sense_context(void *p_excinf)
{
	return(((T_EXCINF *)(p_excinf))->nest_count != 0U);
}

/*
 *  CPU例外の発生した時の割込み優先度マスクの参照
 */
Inline PRI
exc_get_intpri(void *p_excinf)
{
	return((PRI)(((T_EXCINF *)(p_excinf))->intpri));
}

/*
 *  CPUロック状態または割込みロック状態かの参照
 */
Inline bool_t
exc_sense_lock(void *p_excinf)
{
#ifndef TOPPERS_SAFEG_SECURE
	return(((((T_EXCINF *)(p_excinf))->cpsr) & CPSR_INT_MASK) != 0U);
#define CPSR_UNLOCK			UINT_C(0x00)
#else /* TOPPERS_SAFEG_SECURE */
	return(((((T_EXCINF *)(p_excinf))->cpsr) & CPSR_FIQ_BIT) != 0U);
#endif /* TOPPERS_SAFEG_SECURE */
}

/*
 *  CPU例外の発生した時のコンテキストと割込みのマスク状態の参照
 *
 *  CPU例外の発生した時のシステム状態が，カーネル実行中でなく，タスクコ
 *  ンテキストであり，全割込みロック状態でなく，CPUロック状態でなく，割
 *  込み優先度マスク全解除状態である時にtrue，そうでない時にfalseを返す
 *  （CPU例外がカーネル管理外の割込み処理中で発生した場合にもfalseを返
 *  す）．
 *
 */
Inline bool_t
exc_sense_intmask(void *p_excinf)
{
	return(!exc_sense_context(p_excinf)
				&& exc_get_intpri(p_excinf) == TIPM_ENAALL
				&& !exc_sense_lock(p_excinf));
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  MMU関連の操作（VMSA）
 */
#ifdef USE_ARM_MMU

/*
 *  変換テーブルベースレジスタ（TTBR）の設定値
 */
#if __TARGET_ARCH_ARM < 6
#define TTBR_CONFIG		0U
#elif __TARGET_ARCH_ARM < 7
#define TTBR_CONFIG		(CP15_TTBR_RGN_CACHEABLE|CP15_TTBR_RGN_SHAREABLE \
												|CP15_TTBR_RGN_WBACK)
#else /* __TARGET_ARCH_ARM < 7 */
#define TTBR_CONFIG		(CP15_TTBR_RGN_SHAREABLE|CP15_TTBR_RGN_WBWA \
												|CP15_TTBR_IRGN_WBWA)
#endif

/*
 *  レッドゾーン方式を使用する
 */
#define USE_REDZONE
#define TARGET_DUMMY_STKSZ		ARM_PAGE_SIZE

/*
 *  ターゲット依存のユーザスタックのチェック（動的生成機能拡張パッケー
 *  ジ用）
 */
#define CHECK_TARGET_USTACK(ustksz, ustk, p_dominib) do {			\
	CHECK_PAR(SSTKSZ_ALIGN(ustksz));			/*［NGKI1056］*/	\
	CHECK_PAR(SSTACK_ALIGN(ustk));				/*［NGKI1056］*/	\
	CHECK_OBJ(valid_memobj_dom(ustk, ustksz, p_dominib->domptn));	\
												/*［HRPS0241］*/	\
} while (false)

/*
 *  MMUの初期化
 */
#ifndef TOPPERS_MACRO_ONLY

extern void arm_mmu_initialize(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* USE_ARM_MMU */

/*
 *  システム周期オーバラン例外に使用するSVC番号
 */
#define SVC_SCYCOVR		2

#ifndef TOPPERS_MACRO_ONLY

/*
 *  FPUの初期化
 */
#ifdef USE_ARM_FPU
extern void arm_fpu_initialize(void);
#endif /* USE_ARM_FPU */

/*
 *  コア依存の初期化
 */
extern void core_initialize(void);

/*
 *  コア依存の終了処理
 */
extern void core_terminate(void);

/*
 *  タスクの終了
 *
 *  ユーザタスクがメインルーチンからリターンした場合に呼ばれるルーチン．
 *  ユーザタスクから呼び出すため，共有テキスト領域に配置する．
 */
extern void call_ext_tsk(void) NoReturn;

/*
 *  システム周期オーバラン例外の要求
 */
Inline void
raise_scycovr_exception(void)
{
	Asm("svc %0" : : "I"(SVC_SCYCOVR) : "lr");
}

/*
 *  CPU例外の発生状況のログ出力
 */
#ifndef OMIT_XLOG_SYS
extern void xlog_sys(void *p_excinf);
extern void xlog_fsr(uint32_t fsr, uint32_t far);
#endif /* OMIT_XLOG_SYS */

/*
 *  未定義の割込みが入った場合の処理
 */
extern void default_int_handler(void);

/*
 *  未定義の例外が入った場合の処理
 */
extern void default_exc_handler(void *p_excinf, EXCNO excno);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CORE_KERNEL_IMPL_H */
