/*
 *		sil.hのターゲット依存部（EV3RT用）
 *
 *  このヘッダファイルは，sil.hからインクルードされる．他のファイルから
 *  直接インクルードすることはない．このファイルをインクルードする前に，
 *  t_stddef.hがインクルードされるので，それに依存してもよい．
 */

#ifndef TOPPERS_TARGET_SIL_H
#define TOPPERS_TARGET_SIL_H

/*
 *  プロセッサのエンディアン
 */
#define SIL_ENDIAN_LITTLE

/*
 *  コアで共通な定義（チップ依存部は飛ばす）
 */
#include "core_sil.h"

#endif /* TOPPERS_TARGET_SIL_H */
