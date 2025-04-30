/**
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023.All rights reserved.
 *
 * Description: Provides efuse port template \n
 *
 * History: \n
 * 2023-3-4， Create file. \n
 */

#ifndef EFUSE_PORTING_H
#define EFUSE_PORTING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_efuse Efuse
 * @ingroup  drivers_port
 * @{
 */

#define EFUSE_REGION_NUM                    1
#define EFUSE_REGION_MAX_BITS               1024
#define EFUSE_REGION_MAX_BYTES              (EFUSE_REGION_MAX_BITS >> 3)  // MAX_BIT / 8
#define EFUSE_MAX_BITS                      (EFUSE_REGION_MAX_BITS * EFUSE_REGION_NUM)
#define EFUSE_MAX_BYTES                     (EFUSE_MAX_BITS >> 3)  // MAX_BIT / 8
#define EFUSE_MAX_BIT_POS                   8U

#define EFUSE_PMU_CLKLDO_VSET_MASK          0x78
#define EFUSE_PMU_CLKLDO_OFFSET             3
#define EFUSE_TRIM_VERSION_DEFAULT_VALUE    1
#define EFUSE_CHAR_BIT_WIDE                 8
#define EFUSE_CALC_CRC_MAX_LEN              32

#define EFUSE_TRIM_FLAG                     50
#define EFUSE_IS_CHIP_TRIMED                0
// 7-0 -> Version of the trim
#define EFUSE_TRIM_VERSION                  51
// 2-0 -> vset_bbldo[3:1]
// 6-3 -> vset_clkldo[3:0]
#define EFUSE_PMU_TRIM_VSET                 25
#define EFUSE_DIE_ID_BASE_BYTE_ADDR         0

#define EFUSE_FUSING_BYTE_ADDR       37
#define EFUSE_SWD_FUSING             6
#define BURN_EFUSE_IDX_MAX           115
#define EFUSE_BIT_NUM 2048
#ifdef CONFIG_LOADERBOOT_SUPPORT_CUSTOMIZED_READ_EFUSE
#define EFUSE_READ_MAX_BYTE 53
#else
#define EFUSE_READ_MAX_BYTE 32
#endif
#define BURN_EFUSE_BIN_ADDR 0x2000C000
#define EFUSE_CFG_MAX_LEN       1320
#define EFUSE_CFG_MIN_LEN       48

typedef enum {
    HAL_EFUSE_REGION_0,
    HAL_EFUSE_REGION_MAX,
} hal_efuse_region_t;

typedef enum {
    EFUSE_DIE_ID = 0,
    EFUSE_CUSTOM_RESVED_ID = 1,
    EFUSE_IDX_MAX,
} efuse_idx_t;

typedef enum {
    SWD_HW_FUSING,  // 直接从硬件上彻底熔断SWD功能，无法恢复
    SWD_SW_FUSING,  // 熔断SWD功能，但可以软件配置打开，参考接口efuse_swd_debug_enable。
} swd_fusing_t;

/**
 * @if Eng
 * @brief  Base address for the EFUSE boot done.
 * @else
 * @brief  EFUSE的上电完成地址
 * @endif
 */
extern uint32_t g_efuse_boot_done_addr;

/**
 * @if Eng
 * @brief  Base address for the IP.
 * @else
 * @brief  IP的基地址
 * @endif
 */
extern uint32_t g_efuse_base_addr[CONFIG_EFUSE_REGION_NUM];

#if defined(CONFIG_EFUSE_SWITCH_EN)
/**
 * @if Eng
 * @brief  Base address for the EFUSE switch enable.
 * @else
 * @brief  EFUSE的上电开关地址
 * @endif
 */
extern uint32_t g_efuse_switch_en_addr;
#endif

/**
 * @if Eng
 * @brief  Base read address for the IP.
 * @else
 * @brief  IP的读基地址
 * @endif
 */
extern uint32_t g_efuse_region_read_address[CONFIG_EFUSE_REGION_NUM];

/**
 * @if Eng
 * @brief  Base write address for the IP.
 * @else
 * @brief  IP的写基地址
 * @endif
 */
extern uint32_t g_efuse_region_write_address[CONFIG_EFUSE_REGION_NUM];

/**
 * @if Eng
 * @brief  Register hal funcs objects into hal_efuse module.
 * @else
 * @brief  将hal funcs对象注册到hal_efuse模块中
 * @endif
 */
void efuse_port_register_hal_funcs(void);

/**
 * @if Eng
 * @brief  Unregister hal funcs objects from hal_efuse module.
 * @else
 * @brief  从hal_efuse模块注销hal funcs对象
 * @endif
 */
void efuse_port_unregister_hal_funcs(void);

/**
 * @brief  Get the region of a otp byte address
 * @param  byte_addr the addr of the byte to get register
 * @retval region The region of otp
 * @else
 * @brief  获取otp字节地址的区域
 * @param  byte_addr 要获取寄存器的字节的地址
 * @retval 区域OTP的区域
 */
hal_efuse_region_t hal_efuse_get_region(uint32_t byte_addr);

/**
 * @brief  Get the offset addr of a otp byte address
 * @param  byte_addr the addr of the byte to get register
 * @retval address
 * @else
 * @brief  获取otp字节地址的偏移地址
 * @param  byte_addr 要获取寄存器的字节的地址
 * @retval 偏移地址
 */
uint16_t hal_efuse_get_byte_offset(uint32_t byte_addr);


uint32_t efuse_read_item(efuse_idx_t efuse_id, uint8_t *data, uint16_t data_len);

void efuse_port_item_puts(efuse_idx_t efuse_id, uint8_t *data);

/**
 * @brief  Close SWD debug function.
 * @param  [in] type SWD function fusing type.
 * @else
 * @brief  关闭SWD调试功能。
 * @param  [in] type SWD功能熔断类型。
 */
void efuse_close_swd_debug_function(swd_fusing_t type);

/**
 * @brief  SWD debugging enable.
 * @param  [in] en enable or disable.
 * @note   This interface is valid only when the @efuse_close_swd_debug_function parameter is SWD_SW_FUSING.
 * @else
 * @brief  SWD调试功能使能。
 * @param  [in] en 使能或不使能。
 * @note   接口 @efuse_close_swd_debug_function 配置参数为SWD_SW_FUSING时该接口才有用。
 */
void efuse_swd_debug_enable(bool en);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif