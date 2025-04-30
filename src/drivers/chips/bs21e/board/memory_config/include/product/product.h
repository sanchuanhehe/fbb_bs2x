/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  product config
 *
 * Create: 2021-06-16
 */
#ifndef PRODUCT_H
#define PRODUCT_H

#ifndef YES
#define YES (1)
#endif

#ifndef NO
#define NO (0)
#endif

#if (defined BS21E_PRODUCT_NONE)
#define APPLICATION_VERSION_STRING CHIP_BS2X_VERSION
#include "product_none.h"
#endif

#if (defined BS21E_PRODUCT_EVB)
#define APPLICATION_VERSION_STRING CHIP_BS2X_VERSION
#include "product_evb_standard.h"
#endif

#define TWS_EAR_WEAR_NOTICE_SUPP            NO
#define TWS_A2DP_TX_PWR_REDUCE_SUPP         NO
#define TWS_PAGE_TX_PWR_REDUCE_SUPP         NO
#define BT_EXIST                            NO
#define APP_EXIST                           YES
#define DSP_EXIST                           NO
#define DUAL_DSP_EXIST                      NO
#define GNSS_EXIST                          NO
#define SECURITY_EXIST                      NO
#define WIFI_EXIST                          NO
#define UWB_EXIST                           NO
#define NFC_EXIST                           NO
#define CORE_NUMS                           1
#define IPC_SHARE_NUMS                      0
#define OTP_SET_CLK_PERIOD                  NO
#define SHA512_EXIST                        YES
#define SM4_EXIST                           YES
#define SHA_LAST_PACKAGE_IS_BIG_ENDIAN      YES
#define OTP_FIRST_REGION_BITS               1024
#define OTP_SECOND_REGION_BITS              0
#define USE_PMU_WDT                         YES
#define SFC_FLASH_EXIST                     YES
#define SPI_FLASH_EXIST                     YES
#endif