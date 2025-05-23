/**
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * Description: USB Initialize Header. \n
 *
 * History: \n
 * 2023-07-07, Create file. \n
 */
#ifndef USB_INIT_KEYSCAN_APP_H
#define USB_INIT_KEYSCAN_APP_H

#include "implementation/usb_init.h"
#include "gadget/f_hid.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

int usb_init_keyboard_app(device_type dtype);
int usb_deinit_keyscan_app(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif