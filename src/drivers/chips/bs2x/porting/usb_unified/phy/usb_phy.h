/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: USB port Header
 *
 * Create:  2023-01-10
 */

#ifndef _USB_PHY_H
#define _USB_PHY_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    void (*usbStartHcd)(void);
    void (*usbStopHcd)(void);
    void (*usbHost2Device)(void);
    void (*usbDevice2Host)(void);
    void (*usbSuspend)(void);
    void (*usbResume)(void);
} usb_phy_ops_t;

typedef enum {
    USB_SUSPEND,
    USB_RESUME,
    USB_STATUS_MAX
} usb_status_t;

typedef void (*usb_sts_sw_cb_t)(void);

extern void usb_phy_init(void);
extern void usb_phy_reg(const usb_phy_ops_t *usb_phy_ops);
extern void usb_phy_start_hcd(void);
extern void usb_phy_stop_hcd(void);
extern void usb_phy_host_device(void);
extern void usb_phy_device_host(void);
extern void usb_phy_suspend(void);
extern void usb_phy_resume(void);
extern void usb_phy_set_device_state(void);
extern void usb_phy_clear_device_state(void);
extern bool usb_phy_is_device_mode(void);
void usb_register_status_sw_callback(usb_status_t status, usb_sts_sw_cb_t cbk);
void usb_unregister_status_sw_callback(usb_status_t status);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _USB_INIT_H */
