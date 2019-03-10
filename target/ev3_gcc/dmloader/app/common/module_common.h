typedef struct {
    FN         sfncd;
    const void *argument;
    void       *retvalptr;
} MOD_CFG_ENTRY;

typedef struct {
	ATR			cycatr;		/* 周期通知属性 */
	RELTIM		cyctim;		/* 周期通知の通知周期 */
	RELTIM		cycphs;		/* 周期通知の通知位相 */
    MODE        nfymode;
    intptr_t    par1;
    intptr_t    par2;
    intptr_t    epar1;
    intptr_t    epar2;
} T_DML_CCYC;

#define TSFN_CRE_TSK (1)
#define TSFN_CRE_SEM (2)
#define TSFN_CRE_FLG (3)
#define TSFN_CRE_DTQ (4)
#define TSFN_CRE_PDQ (5)
#define TSFN_CRE_MTX (6)
#define TSFN_CRE_CYC (7)
