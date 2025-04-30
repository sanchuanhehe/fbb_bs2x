/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pinctrl.h"
#include "i2c.h"
#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include "soc_osal.h"
#include "app_init.h"

#define CONFIG_I2C_MASTER_SCL_PIN_MODE 26
#define CONFIG_I2C_MASTER_SDA_PIN_MODE 27
#define CONFIG_I2C_SCL_MASTER_PIN 0
#define CONFIG_I2C_SDA_MASTER_PIN 1
#define I2C_MASTER_ADDR 0x0
#define I2C_SLAVE1_ADDR 0x38
#define I2C_SET_BANDRATE 400000
#define I2C_TASK_STACK_SIZE 0x800
#define I2C_TASK_PRIO 24

void app_i2c_init_pin(void)
{
    uapi_pin_set_mode(CONFIG_I2C_SCL_MASTER_PIN, CONFIG_I2C_MASTER_SCL_PIN_MODE);
    uapi_pin_set_mode(CONFIG_I2C_SDA_MASTER_PIN, CONFIG_I2C_MASTER_SDA_PIN_MODE);
}

void OledTask(void)
{
    uint32_t baudrate = I2C_SET_BANDRATE;
    uint8_t hscode = I2C_MASTER_ADDR;
    app_i2c_init_pin();
    errcode_t ret = uapi_i2c_master_init(0, baudrate, hscode);
    if (ret != 0) {
        printf("i2c init failed, ret = %0x\r\n", ret);
    }

    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_DrawString("Hello World!!!", Font_7x10, White);
    ssd1306_UpdateScreen();
}

void oled_entry(void)
{
    uint32_t ret;
    osal_task *taskid;
    // 创建任务调度
    osal_kthread_lock();
    // 创建任务1
    taskid = osal_kthread_create((osal_kthread_handler)OledTask, NULL, "OledTask", I2C_TASK_STACK_SIZE);
    ret = osal_kthread_set_priority(taskid, I2C_TASK_PRIO);
    if (ret != OSAL_SUCCESS) {
        printf("create task1 failed .\n");
    }
    osal_kthread_unlock();
}

app_run(oled_entry);