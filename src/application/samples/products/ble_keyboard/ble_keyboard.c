/**
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * Description: BLE KEYBOARD Sample Source. \n
 *
 * History: \n
 * 2023-07-10, Create file. \n
 */
#include "common_def.h"
#include "soc_osal.h"
#include "securec.h"
#include "app_init.h"
#include "keyscan.h"
#include "ble_keyboard_server.h"
#include "ble_keyboard_server/ble_hid_keyboard_server.h"

#define BLE_KEYBOARD_MAX_KEY_LENTH              6
#define BLE_KEYBOARD_REPORT_LENTH               7
#define USB_HID_SPECIAL_KEY_MIN                 0xE0
#define USB_HID_SPECIAL_KEY_MAX                 0xE7

#define BLE_KEYBOARD_TASK_PRIO                  28
#define BLE_KEYBOARD_TASK_STACK_SIZE            0x600

typedef struct usb_hid_keyboard_report {
    uint8_t special_key;                         /*!< 8bit special key(Lctrl Lshift Lalt Lgui Rctrl Rshift Ralt Rgui) */
    uint8_t key[BLE_KEYBOARD_MAX_KEY_LENTH]; /*!< Normal key */
} usb_hid_keyboard_report_t;

static usb_hid_keyboard_report_t g_send_key_msg;

#if defined(CONFIG_KEYSCAN_USE_FULL_KEYS_TYPE)
uint8_t g_ble_keyboard_key_map[CONFIG_KEYSCAN_ENABLE_ROW][CONFIG_KEYSCAN_ENABLE_COL] = {
    { 0x29, 0x2B, 0x14, 0x35, 0x04, 0x1E, 0x1D, 0x00 },
    { 0x3D, 0x3C, 0x08, 0x3B, 0x07, 0x20, 0x06, 0x00 },
    { 0x00, 0x39, 0x1A, 0x3A, 0x16, 0x1F, 0x1B, 0x00 },
    { 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0xE4, 0x00 },
    { 0x0A, 0x17, 0x15, 0x22, 0x09, 0x21, 0x19, 0x05 },
    { 0x0B, 0x1C, 0x18, 0x23, 0x0D, 0x24, 0x10, 0x11 },
    { 0x3F, 0x30, 0x0C, 0x2E, 0x0E, 0x25, 0x36, 0x00 },
    { 0x00, 0x00, 0x12, 0x40, 0x0F, 0x26, 0x37, 0x00 },
    { 0x34, 0x2F, 0x13, 0x2D, 0x33, 0x27, 0x00, 0x38 },
    { 0x3E, 0x2A, 0x00, 0x41, 0x31, 0x42, 0x28, 0x2C },
    { 0x00, 0x00, 0xE3, 0x00, 0x00, 0x43, 0x00, 0x51 },
    { 0xE2, 0x00, 0x00, 0x00, 0x00, 0x45, 0xE5, 0xE6 },
    { 0x00, 0x53, 0x00, 0x00, 0xE1, 0x44, 0x00, 0x4F },
    { 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x50 },
    { 0x5F, 0x5C, 0x61, 0x5E, 0x59, 0x62, 0x55, 0x5B },
    { 0x54, 0x60, 0x56, 0x57, 0x5D, 0x5A, 0x58, 0x63 },
};
#else
static uint8_t g_ble_keyboard_key_map[CONFIG_KEYSCAN_ENABLE_ROW][CONFIG_KEYSCAN_ENABLE_COL]  = {
    { 0x04, 0x05 },
    { 0x06, 0x07 },
    { 0x08, 0x09 },
};
#endif /* CONFIG_KEYSCAN_USE_FULL_KEYS_TYPE */

static int ble_keyscan_callback(int argc, uint8_t argv[])
{
    uint8_t normal_key_num = BLE_KEYBOARD_MAX_KEY_LENTH - 1;
    uint8_t tmp_key = 0;

    if (memset_s(&g_send_key_msg, sizeof(g_send_key_msg), 0, sizeof(g_send_key_msg)) != EOK) {
        return 0;
    }

    for (int i = 0; i < argc; i++) {
        tmp_key = argv[i];
        if (tmp_key >= USB_HID_SPECIAL_KEY_MIN && tmp_key <= USB_HID_SPECIAL_KEY_MAX) {
            g_send_key_msg.special_key |= (1 << (tmp_key - USB_HID_SPECIAL_KEY_MIN));
        } else {
            g_send_key_msg.key[normal_key_num] = tmp_key;
            normal_key_num--;
        }
    }
    ble_hiddev_keyboard_server_send_input_report_by_uuid((uint8_t *)(uintptr_t)&g_send_key_msg,
        BLE_KEYBOARD_REPORT_LENTH);
    return 1;
}

void ble_keyboard_init(void)
{
    uapi_set_keyscan_value_map((uint8_t **)g_ble_keyboard_key_map,
                               CONFIG_KEYSCAN_ENABLE_ROW, CONFIG_KEYSCAN_ENABLE_COL);
    uapi_keyscan_init(EVERY_ROW_PULSE_40_US, HAL_KEYSCAN_MODE_0, KEYSCAN_INT_VALUE_RDY);
    uapi_keyscan_register_callback(ble_keyscan_callback);
    uapi_keyscan_enable();
    osal_printk("keyboard init done\r\n");
}

static void *ble_keyboard_task(const char *arg)
{
    unused(arg);
    ble_keyboard_server_init();
    ble_keyboard_init();
    return NULL;
}

static void ble_keyboard_entry(void)
{
    osal_task *task_handle = NULL;
    osal_kthread_lock();
    task_handle = osal_kthread_create((osal_kthread_handler)ble_keyboard_task, 0, "BLEKeyboardTask",
                                      BLE_KEYBOARD_TASK_STACK_SIZE);
    if (task_handle != NULL) {
        osal_kthread_set_priority(task_handle, BLE_KEYBOARD_TASK_PRIO);
    }
    osal_kthread_unlock();
}

/* Run the ble_keyboard_entry. */
app_run(ble_keyboard_entry);