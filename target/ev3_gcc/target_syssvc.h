#pragma once

/*
 *  起動メッセージのターゲットシステム名
 */
#define TARGET_NAME "EV3(AM1808(ARM926ES-J))"

/*
 *  システムログの低レベル出力のための文字出力
 *
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．
 */
extern void	target_fput_log(char c);

/*
 *  ID of SIO ports
 */
#define SIO_PORT_UART               (1)
#define SIO_PORT_BT                 (2)
#define SIO_PORT_LCD                (3)
#define SIO_PORT_SPP_MASTER_TEST    (4)
#define TNUM_PORT                   (4)

/*
 *  Default SIO Port for syslog etc.
 */
extern int SIO_PORT_DEFAULT;

/*
 *  Syslog
 */
#define LOGTASK_PORTID              (SIO_PORT_DEFAULT)
#define LOGTASK_STACK_SIZE	        (4096) // TODO: NOTE: default (1024) is not enough for sample1
#define TCNT_SYSLOG_BUFFER          (1024)

/*
 *  Utility function for outputting SVC error
 */
#if defined(LOG_ERROR)
#define SVC_PERROR(expr) svc_perror(__FILE__, __LINE__, #expr, (expr))
static inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd) {
    if (ercd < 0) {
        t_perror(LOG_ERROR, file, line, expr, ercd);
    }
}
#endif
