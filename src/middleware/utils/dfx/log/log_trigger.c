/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:  LOG TRIGGER MODULE
 *
 * Create:
 */

#include "log_trigger.h"
#include "dfx_adapt_layer.h"

#if ((CORE == CORE_LOGGING) && defined(BUILD_APPLICATION_STANDARD)) || (CORE_NUMS  == 1)

#ifndef USE_CMSIS_OS
#error "log reader not implemented for non-os version"
#endif

static log_trigger_callback_t g_log_trigger_callback = NULL;

void log_trigger(void)
{
    if (g_log_trigger_callback != NULL) {
        g_log_trigger_callback();
    }
}

void register_log_trigger(log_trigger_callback_t callback)
{
    if (callback != NULL) {
        g_log_trigger_callback = callback;
    }
}

#else  // (CORE != CORE_LOGGING) case
#include "ipc.h"
#ifdef IPC_NEW
#include "ipc_porting.h"
#endif

// Have a definition for the right cores_t enum in CORES_CORE_LOGGING
#if CORE_LOGGING == BT
#define CORES_CORE_LOGGING CORES_BT_CORE
#elif CORE_LOGGING == PROTOCOL
#define CORES_CORE_LOGGING CORES_PROTOCOL_CORE
#elif CORE_LOGGING == APPS
#define CORES_CORE_LOGGING CORES_APPS_CORE
#elif CORE_LOGGING == GNSS
#define CORES_CORE_LOGGING CORES_GNSS_CORE
#endif

#ifdef IPC_NEW
void log_trigger(void)
{
    ipc_msg_info_t head;
    head.dst_core = CORES_CORE_LOGGING;
    head.priority = IPC_PRIORITY_LOWEST;
    head.msg_id = IPC_MSG_LOG_INFO;
    head.buf_addr = NULL;
    head.buf_len = 0;
    head.channel = 0;
    (void)uapi_ipc_send_msg_async(&head);
}
#elif (defined CONFIG_DFX_SUPPORT_CUSTOM_LOG) && (CONFIG_DFX_SUPPORT_CUSTOM_LOG == DFX_YES)
void log_trigger(void)
{
    dfx_log_trigger();
}
#else
void log_trigger(void)
{
#if (BTH_WITH_SMART_WEAR == NO) && (USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == YES)
    if (ipc_check_status(CORES_CORE_LOGGING) == IPC_STATUS_OK) {
        ipc_interrupt_core(CORES_CORE_LOGGING);
    }
#else
    (void)ipc_send_message(CORES_CORE_LOGGING,
                           IPC_ACTION_LOG_INFO,
                           NULL,
                           0,
                           IPC_PRIORITY_LOWEST, false);
#endif
}
#endif
#endif  // (CORE == CORE_LOGGING)
