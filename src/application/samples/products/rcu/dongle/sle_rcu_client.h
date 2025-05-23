/**
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * Description: SLE RCU sample of client. \n
 *
 * History: \n
 * 2023-09-21, Create file. \n
 */
#ifndef SLE_RCU_CLIENT_H
#define SLE_RCU_CLIENT_H

#include "sle_ssap_client.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void sle_rcu_client_init(ssapc_notification_callback notification_cb, ssapc_indication_callback indication_cb);
void sle_rcu_start_scan(void);
uint16_t get_sle_rcu_conn_id(void);
ssapc_write_param_t *get_sle_rcu_send_param(void);
uint8_t get_ssap_find_ready(void);
uint8_t get_sle_rcu_get_connect_state(void);
uint8_t get_ssap_connect_param_update_ready(void);
#if defined (CONFIG_RCU_MASS_PRODUCTION_TEST)
sle_addr_t *sle_rcu_report_client_addr(void);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif