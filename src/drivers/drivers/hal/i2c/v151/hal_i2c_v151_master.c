/**
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * Description: Provides HAL i2c \n
 *
 * History: \n
 * 2022-08-15, Create file. \n
 */

#include <stdint.h>
#include "securec.h"
#include "common_def.h"
#include "i2c_porting.h"
#include "hal_i2c.h"
#include "hal_i2c_v151_comm.h"
#include "hal_i2c_v151_master.h"

#define HAL_I2C_SS_MIN_SCL_LCNT 13
#define HAL_I2C_SS_MIN_SCL_HCNT 14
#define HAL_I2C_FS_MIN_SCL_LCNT 16
#define HAL_I2C_FS_MIN_SCL_HCNT 14
#define HAL_I2C_HS_MIN_SCL_LCNT 22
#define HAL_I2C_HS_MIN_SCL_HCNT 14

#define HAL_I2C_LCNT_FIXED_OFFSET 1
#define HAL_I2C_HCNT_FIXED_OFFSET 8
#define MAX_I2C_GET_COUNT_TIMES 0xFFFF

static bool hal_i2c_cfg_taget_address(i2c_bus_t bus, uint16_t addr, hal_i2c_v151_addr_permission_t permission)
{
    hal_i2c_v151_addr_cfg_attr_t cfg_attr;

    bool cfg_valid = hal_i2c_v151_get_address_cfg(addr, permission, &cfg_attr);
    if (cfg_valid == false) {
        return false;
    }

    hal_i2c_v151_set_ic_disable(bus);
    hal_i2c_v151_set_con_master_address_mode(bus, cfg_attr.addr_width);

    if (cfg_attr.addr_type == I2C_V151_ADDR_TYPE_FOR_GC) {
        hal_i2c_v151_set_gc_mode(bus);
    } else {
        hal_i2c_v151_set_normal_mode(bus);
        hal_i2c_v151_set_master_target_address(bus, cfg_attr.real_addr);
    }
    hal_i2c_v151_set_ic_enable(bus);
    return true;
}

static hal_i2c_speed_mode_t hal_i2c_v151_get_speed_mode(uint32_t baudrate)
{
    if (baudrate <= I2C_SS_MODE_BAUDRATE_HIGH_LIMIT) {
        return I2C_SPEED_MODE_SS;
    } else if (baudrate <= I2C_FS_MODE_BAUDRATE_HIGH_LIMIT) {
        return I2C_SPEED_MODE_FS;
    }
#ifdef CONFIG_I2C_SUPPORT_FAST_PLUS_SPEED
    if (baudrate <= I2C_FPS_MODE_BAUDRATE_HIGH_LIMIT) {
        return I2C_SPEED_MODE_FPS;
    }
#endif
    return I2C_SPEED_MODE_HS;
}

static void hal_i2c_v151_calc_scl_cnt(const hal_i2c_v151_scl_cnt_calc_input_t *input,
                                      hal_i2c_v151_scl_cnt_calc_output_t *output)
{
    /* ref scl_cnt @Databook */
    static hal_i2c_v151_scl_region_t scl_cnt_min[I2C_SPEED_MODE_MAX_NUM] = {
        {HAL_I2C_SS_MIN_SCL_LCNT, HAL_I2C_SS_MIN_SCL_HCNT},
        {HAL_I2C_FS_MIN_SCL_LCNT, HAL_I2C_FS_MIN_SCL_HCNT},
        {HAL_I2C_HS_MIN_SCL_LCNT, HAL_I2C_HS_MIN_SCL_HCNT}
    };

    uint32_t min_lcnt = scl_cnt_min[input->speed_mode].lcnt;
    uint32_t min_hcnt = scl_cnt_min[input->speed_mode].hcnt;

    uint32_t total_cnt = (input->freq + input->baudrate) / input->baudrate;
    uint32_t lcnt_add = 0;
    uint32_t hcnt_add = 0;

    if (total_cnt > (min_lcnt + min_hcnt)) {
        lcnt_add = (total_cnt - (min_lcnt + min_hcnt)) >> 1;
        hcnt_add = total_cnt - (min_lcnt + min_hcnt) - lcnt_add;
    }

    output->hcnt = min_hcnt + hcnt_add - HAL_I2C_HCNT_FIXED_OFFSET;
    output->lcnt = min_lcnt + lcnt_add - HAL_I2C_LCNT_FIXED_OFFSET;
}

static void hal_i2c_v151_set_scl_cnt(i2c_bus_t bus, hal_i2c_speed_mode_t speed_mode,
                                     hal_i2c_v151_scl_cnt_calc_output_t *output)
{
    if (speed_mode == I2C_SPEED_MODE_SS) {
        hal_i2c_v151_set_ss_scl_hcnt(bus, output->hcnt);
        hal_i2c_v151_set_ss_scl_lcnt(bus, output->lcnt);
    } else if (speed_mode == I2C_SPEED_MODE_FS) {
        hal_i2c_v151_set_fs_scl_hcnt(bus, output->hcnt);
        hal_i2c_v151_set_fs_scl_lcnt(bus, output->lcnt);
    } else {
        hal_i2c_v151_set_hs_scl_hcnt(bus, output->hcnt);
        hal_i2c_v151_set_hs_scl_lcnt(bus, output->lcnt);
    }
}

static void hal_i2c_v151_master_cfg_baudrate(i2c_bus_t bus, hal_i2c_speed_mode_t speed_mode, uint32_t baudrate)
{
    hal_i2c_v151_scl_cnt_calc_input_t input;
    hal_i2c_v151_scl_cnt_calc_output_t output = { 0 };
    input.freq = i2c_port_get_clock_value(bus);
    input.speed_mode = speed_mode;
    input.baudrate = baudrate;

    hal_i2c_v151_calc_scl_cnt(&input, &output);
    hal_i2c_v151_set_scl_cnt(bus, speed_mode, &output);
}

static errcode_t hal_i2c_v151_master_write_prepeare(i2c_bus_t bus, uintptr_t param)
{
    hal_i2c_prepare_config_t *cfg = (hal_i2c_prepare_config_t *)param;

    bool rst = hal_i2c_cfg_taget_address(bus, cfg->addr, I2C_V151_ADDR_PERMISION_MASTER);
    if (!rst) {
        return ERRCODE_I2C_PERMISSION_INVALID;
    }
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    hal_i2c_v151_com_clr_int(bus);
#endif
    hal_i2c_v151_get_ctrl_info(bus)->write_operate_type = cfg->operation_type;
    if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_INT) {
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
        hal_i2c_v151_com_prepare_int(bus);
        hal_i2c_v151_prepare_int_tx(bus);
#endif
    } else if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_DMA) {
#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
        hal_i2c_v151_set_transmit_dma_en(bus, true);
        hal_i2c_v151_set_transmit_dma_level(bus, HAL_I2C_V151_TX_DMA_TL);
#endif
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v151_master_write_restore(i2c_bus_t bus, uintptr_t param)
{
    unused(bus);
    unused(param);
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    hal_i2c_v151_mask_int(bus, I2C_V151_INT_TX_EMPTY);
#endif
#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
    hal_i2c_v151_set_transmit_dma_en(bus, false);
#endif
    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v151_master_read_prepeare(i2c_bus_t bus, uintptr_t param)
{
    hal_i2c_prepare_config_t *cfg = (hal_i2c_prepare_config_t *)param;

    bool rst = hal_i2c_cfg_taget_address(bus, cfg->addr, I2C_V151_ADDR_PERMISION_SLAVE);
    if (!rst) {
        return ERRCODE_I2C_PERMISSION_INVALID;
    }
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    hal_i2c_v151_com_clr_int(bus);
#endif
    hal_i2c_v151_get_ctrl_info(bus)->read_operate_type = cfg->operation_type;
    if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_INT) {
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
        hal_i2c_v151_prepare_int_rx(bus);
#endif
    } else if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_DMA) {
#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
        hal_i2c_v151_set_transmit_dma_level(bus, HAL_I2C_V151_TX_DMA_TL);
        hal_i2c_v151_set_receive_dma_level(bus, HAL_I2C_V151_RX_DMA_TL - 1);

        hal_i2c_v151_set_transmit_dma_en(bus, true);
        hal_i2c_v151_set_receive_dma_en(bus, true);
#endif
    }

    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v151_master_read_restore(i2c_bus_t bus, uintptr_t param)
{
    unused(bus);
    unused(param);
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    hal_i2c_v151_com_clr_int(bus);
    hal_i2c_v151_com_restore_int(bus);
    hal_i2c_v151_restore_int_rx(bus);
#endif
#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
    hal_i2c_v151_set_transmit_dma_en(bus, false);
    hal_i2c_v151_set_receive_dma_en(bus, false);
#endif
    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v151_master_check_tx_available(i2c_bus_t bus, uintptr_t param)
{
    ic_v151_status_data_t status;
    status.d32 = hal_i2c_v151_get_status(bus);
    *(uint32_t *)param = status.b.tfnf;
    return ERRCODE_SUCC;
}

static uint32_t hal_i2c_v151_txfifo_not_full(i2c_bus_t bus)
{
    ic_v151_status_data_t status;
    status.d32 = hal_i2c_v151_get_status(bus);
    return status.b.tfnf;
}

static bool hal_i2c_v151_check_timeout_by_count(uint32_t *trans_time, uint32_t timeout)
{
    (*trans_time)++;
    return ((*trans_time > timeout) ? true : false);
}

static errcode_t hal_i2c_v151_master_send_cmd(i2c_bus_t bus, uintptr_t param)
{
    hal_i2c_buffer_wrap_t *read_data = (hal_i2c_buffer_wrap_t *)param;
    if (read_data->len == 0) {
        return ERRCODE_I2C_RECEIVE_PARAM_INVALID;
    }

    uint32_t start_time = 0;
    uint32_t len = read_data->len;
    hal_i2c_v151_update_rx_tl(bus, len);
    uint32_t loop = 0;
    do {
        if (hal_i2c_v151_check_timeout_by_count(&start_time, MAX_I2C_GET_COUNT_TIMES)) {
            return ERRCODE_I2C_TIMEOUT;
        }
        if (hal_i2c_v151_txfifo_not_full(bus) != 0) {
            uint32_t first_flag = (loop == 0);
            uint32_t last_flag = ((loop + 1) == len);
            uint32_t restart_flag = read_data->restart_flag;
            uint32_t stop_flag = read_data->stop_flag;

            uint32_t read_cmd = hal_i2c_v151_get_data_read_cmd(first_flag, restart_flag, last_flag, stop_flag);
            hal_i2c_v151_set_read_direction(bus, read_cmd);
            loop++;
        }
    } while (loop < len);
    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v151_master_check_rx_transmit_end(i2c_bus_t bus, uintptr_t param)
{
    *(uint32_t *)param = hal_i2c_v151_get_int(bus, I2C_V151_RAW_INTR_STAT, I2C_V151_INT_STOP_DET);
    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v151_master_check_reastart_ready(i2c_bus_t bus, uintptr_t param)
{
    unused(bus);
    *(uint32_t *)param = true;
    return ERRCODE_SUCC;
}

static hal_i2c_inner_ctrl_t g_hal_i2c_master_ctrl_func_array[I2C_CTRL_MAX] = {
    hal_i2c_v151_master_write_prepeare,               /* I2C_CTRL_WRITE_PREPARE */
    hal_i2c_v151_master_write_restore,                /* I2C_CTRL_WRITE_RESTORE */
    hal_i2c_v151_master_read_prepeare,                /* I2C_CTRL_READ_PREPARE */
    hal_i2c_v151_master_read_restore,                 /* I2C_CTRL_READ_RESTORE */
    hal_i2c_v151_get_write_num,                       /* I2C_CTRL_GET_WRITE_NUM */
    hal_i2c_v151_get_read_num,                        /* I2C_CTRL_GET_READ_NUM */
    hal_i2c_v151_master_check_tx_available,           /* I2C_CTRL_CHECK_TX_AVAILABLE */
    hal_i2c_v151_check_rx_available,                  /* I2C_CTRL_CHECK_RX_AVAILABLE */
    hal_i2c_v151_master_send_cmd,                     /* I2C_CTRL_FLUSH_RX_FIFO */
    hal_i2c_v151_check_tx_transmit_end,               /* I2C_CTRL_CHECK_TX_PROCESS_DONE */
    hal_i2c_v151_master_check_rx_transmit_end,        /* I2C_CTRL_CHECK_RX_PROCESS_DONE */
    hal_i2c_v151_master_check_reastart_ready,         /* I2C_CTRL_CHECK_RESTART_READY */
    hal_i2c_v151_check_transmit_abrt,                 /* I2C_CTRL_CHECK_TRANSMIT_ABRT */
    hal_i2c_v151_get_data_addr,                       /* I2C_CTRL_GET_DMA_DATA_ADDR */
    hal_i2c_v151_check_tx_fifo_empty,                 /* I2C_CTRL_CHECK_TX_FIFO_EMPTY */
};

#pragma weak hal_i2c_master_init = hal_i2c_v151_master_init
errcode_t hal_i2c_v151_master_init(i2c_bus_t bus,
    uint32_t baudrate, uint8_t hscode, hal_i2c_callback_t callback)
{
#ifdef CONFIG_I2C_NOT_SUPPORT_HIGH_SPEED
    unused(hscode);
#endif
    hal_i2c_v151_regs_init();

    hal_i2c_v151_deinit(bus);

    hal_i2c_v151_init_comp_param(bus);
    hal_i2c_v151_set_con_master_mode(bus);
    hal_i2c_v151_set_con_master_address_mode(bus, I2C_V151_7_BITS_ADDR);
    hal_i2c_v151_set_con_rx_full_hold(bus, IC_V151_CON_ENABLED_BIT);

    /* First ensure baudrate is valid, the cfg mode and code, make sure the cfg can be restore */
    hal_i2c_speed_mode_t speed_mode = hal_i2c_v151_get_speed_mode(baudrate);
    hal_i2c_v151_master_cfg_baudrate(bus, speed_mode, baudrate);
    hal_i2c_v151_set_speed_mode(bus, (uint32_t)speed_mode + 1);

#ifdef CONFIG_I2C_SUPPORT_FAST_PLUS_SPEED
    if (speed_mode == I2C_SPEED_MODE_FPS) {
        hal_i2c_v151_set_hs_address_code(bus, hscode);
    }
#endif

    if (speed_mode == I2C_SPEED_MODE_HS) {
#ifdef CONFIG_I2C_NOT_SUPPORT_HIGH_SPEED
        return ERRCODE_I2C_RATE_INVALID;
#else
        hal_i2c_v151_set_hs_address_code(bus, hscode);
#endif
    }
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    hal_i2c_v151_mask_all_int(bus);
    hal_i2c_v151_com_clr_int(bus);
#endif
    hal_i2c_v151_set_ic_enable(bus);
    unused(callback);
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    hal_i2c_v151_register_callback(callback);
#endif
    hal_i2c_v151_load_ctrl_func(bus, g_hal_i2c_master_ctrl_func_array);
    return ERRCODE_SUCC;
}