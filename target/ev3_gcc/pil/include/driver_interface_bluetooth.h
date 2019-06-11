/**
 * Interface for Bluetooth driver
 */

#pragma once

/**
 * Function code for extended service calls
 */
#define TFN_SPP_MASTER_RESET (42)
#define TFN_SPP_MASTER_CONNECT (43)

/**
 * Extended service call wrappers which can be used to implement APIs
 */

static inline ER spp_master_reset() {
	ER_UINT ercd = cal_svc(TFN_SPP_MASTER_RESET, 0, 0, 0, 0, 0);
	assert(ercd != E_NOMEM);
	return ercd;
}
static inline ER spp_master_connect(const uint8_t *addr, const char *pin, const char *service) {
	ER_UINT ercd = cal_svc(TFN_SPP_MASTER_CONNECT, (intptr_t)addr, (intptr_t)pin, (intptr_t)service, 0, 0);
	assert(ercd != E_NOMEM);
	return ercd;
}

/**
 * Extended Service Call Stubs
 */
extern ER_UINT extsvc_spp_master_reset(intptr_t par1, intptr_t par2, intptr_t par3, intptr_t par4, intptr_t par5, ID cdmid);
extern ER_UINT extsvc_spp_master_connect(intptr_t addr, intptr_t pin, intptr_t service, intptr_t par4, intptr_t par5, ID cdmid);
