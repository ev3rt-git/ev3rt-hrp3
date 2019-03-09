#include "kernel/kernel_impl.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"

// Log priority for exception handling
#define LOG_EXCEPT LOG_ERROR

/*
 *  Terminator task
 */

void
platform_terminator_task(intptr_t unused) {
    ER ercd;
    intptr_t data;
    while (1) {
        SVC_PERROR(ercd = rcv_dtq(PLATFORM_TERM_DTQ, &data));
        if (ercd != E_OK) continue;

        assert(data > 0);
		syslog(LOG_EXCEPT, "Terminate task (tid = %d) for recovery.", data);
        SVC_PERROR(ter_tsk(data));
    }
}

/*
 *  Segment information
 */

#define TMAX_SEGINFO_NUM (10U)

typedef struct {
    const char  *name;
    uintptr_t   paddr;
    size_t      size;
} seginfo_t;

static seginfo_t seginfos[TMAX_SEGINFO_NUM];
static size_t seginfo_num = 0;

void
platform_add_segment_information(const char *name, uintptr_t paddr, size_t size) {
    if (seginfo_num >= TMAX_SEGINFO_NUM) {
        syslog(LOG_ERROR, "Segment information cannot be added.");
        return;
    }
    seginfos[seginfo_num].name = name;
    seginfos[seginfo_num].paddr = paddr;
    seginfos[seginfo_num].size = size;
    seginfo_num++;
}

/*
 *  Exception handlers
 */

static void
xlog_sys_mod(void *p_excinf) {
    uint32_t r0         = ((T_EXCINF *)(p_excinf))->r0;
    uint32_t r1         = ((T_EXCINF *)(p_excinf))->r1;
    uint32_t r2         = ((T_EXCINF *)(p_excinf))->r2;
    uint32_t r3         = ((T_EXCINF *)(p_excinf))->r3;
    uint32_t r12        = ((T_EXCINF *)(p_excinf))->r12;
    uint32_t lr         = ((T_EXCINF *)(p_excinf))->lr;
    uint32_t pc         = ((T_EXCINF *)(p_excinf))->pc;

    if (seginfo_num > 0) {
	    syslog(LOG_EXCEPT, "Segment information:");
        for(size_t i = 0; i < seginfo_num; i++) {
	        syslog(LOG_EXCEPT, "%s segment physical address: 0x%08x, size: %d",
                seginfos[i].name, seginfos[i].paddr, seginfos[i].size);
        }
    }

	static const char *regstr[] = { "r0", "r1", "r2", "r3", "r12", "lr", "pc" };
	uint32_t           regval[] = {  r0,   r1,   r2,   r3,   r12,   lr,   pc  };

    syslog(LOG_EXCEPT, "Registers in segment:");
    bool_t has_register_in_segment = false;
	for (size_t i = 0; i < sizeof(regval) / sizeof(uint32_t); i++) {
        for (size_t j = 0; j < seginfo_num; j++) {
		    if (regval[i] >= seginfos[j].paddr && regval[i] < seginfos[j].paddr + seginfos[j].size) {
		        syslog(LOG_EXCEPT, "%s = 0x%08x, virtual address: 0x%08x in %s segment", regstr[i], regval[i], regval[i] - seginfos[j].paddr, seginfos[j].name);
                has_register_in_segment = true;
            }
        }
	}
    if (!has_register_in_segment) {
        syslog(LOG_EXCEPT, "no register has address in segments");
    }

	syslog(LOG_EXCEPT, "Raw exception frame:");
	syslog_4(LOG_EXCEPT, "pc = %08x, cpsr = %08x, lr = %08x, r12 = %08x",
			((T_EXCINF *)(p_excinf))->pc, ((T_EXCINF *)(p_excinf))->cpsr,
			((T_EXCINF *)(p_excinf))->lr, ((T_EXCINF *)(p_excinf))->r12);
	syslog_4(LOG_EXCEPT, "r0 = %08x, r1 = %08x, r2 = %08x, r3 = %08x",
			((T_EXCINF *)(p_excinf))->r0, ((T_EXCINF *)(p_excinf))->r1,
			((T_EXCINF *)(p_excinf))->r2, ((T_EXCINF *)(p_excinf))->r3);
	syslog_3(LOG_EXCEPT, "nest_count = %d, intpri = %d, rundom = %08x",
			((T_EXCINF *)(p_excinf))->nest_count,
			((T_EXCINF *)(p_excinf))->intpri,
			((T_EXCINF *)(p_excinf))->rundom);
}

void platform_exception_handler(void *p_excinf, EXCNO excno) {
	syslog(LOG_EXCEPT, "====================EXCEPTION DETECTED====================");

    uint32_t fsr = 0U, far = 0xdeafbeefU;
	ID tid = TSK_NONE;
#define CP15_READ_IFSR(reg)		Asm("mrc p15, 0, %0, c5, c0, 1":"=r"(reg))
    switch (excno) {
	case EXCNO_PABORT:
		syslog_0(LOG_EXCEPT, "Prefetch Abort exception occurs.");
        CP15_READ_IFSR(fsr);
        /* NOTE: no IFAR for PABORT in ARM926 */
		break;
	case EXCNO_DABORT:
		syslog_0(LOG_EXCEPT, "Data Abort exception occurs.");
	    CP15_READ_FSR(fsr);
	    CP15_READ_FAR(far);
		break;
    default:
	    syslog_1(LOG_EXCEPT, "Unregistered exception %d occurs.", excno);
	    ext_ker();
    }
	SVC_PERROR(get_tid(&tid));

    // Print debug information
    syslog(LOG_EXCEPT, "Task ID: %d", tid);
    xlog_fsr(fsr, far);
	xlog_sys_mod(p_excinf);


    if (tid != TSK_NONE) {
        SVC_PERROR(psnd_dtq(PLATFORM_TERM_DTQ, tid));
	} else {
		syslog(LOG_EXCEPT, "Fatal error (tid = %d), exit kernel.", tid);
		ext_ker();
	}

	syslog(LOG_EXCEPT, "==========================================================");
}

