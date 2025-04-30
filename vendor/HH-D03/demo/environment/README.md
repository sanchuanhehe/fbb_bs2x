# environment

## 1.1 介绍

**功能介绍：** 在SSD1306 OLED屏幕实时显示当前环境温湿度。

**软件概述：** I2C是一种串行通信协议，允许将多个设备连接到一条总线上。每个连接到总线上的器件都有唯一地址，任何器件既可作为主机也可以作为从机，但同一时刻只允许有一个主机。

**硬件概述：** 由于BS21E开发板IO口功能复用（全路由，任意IO口都可以复用全部功能）。OLED数据手册参考：https://gitee.com/HiSpark/hi3861_hdu_iot_application/issues/I6WPSS?from=project-issue 里面的液晶显示器.pdf。 ;环境监测板AHT20数据手册参考https://gitee.com/HiSpark/hi3861_hdu_iot_application/issues/I6WPSS?from=project-issue 里面的AHT20.pdf，BS21E开发板IO0接底板RX，IO1接底板TX，硬件搭建要求如图所示：

参考[OLED板原理图](../../../../docs/hardware/HH-D03/HiSpark_WiFi_IoT_OLED_VER.A.pdf)、[环境监测原理图](../../../../docs/hardware/HH-D03/HiSpark_WiFi_IoT_EM_VER.A.pdf)、[底板原理图](../../../../docs/hardware/HH-D03/HiSpark_WiFi_IoT_EXB_VER.A.pdf)、[核心板原理图](../../../../docs/hardware/HH-D03/HH-D03_原理图_V01.pdf)

![image-20250424151214774](../../../../docs/pic/environment/image-20250424151214774.png)

## 1.2 约束与限制

### 1.2.1 支持应用运行的芯片和开发板

  本示例支持开发板：HH-D03

### 1.2.2 支持API版本、SDK版本

  本示例支持版本号：1.0.15以上

### 1.2.3 支持IDE版本

  本示例支持IDE版本号：1.0.0.10以上；

## 1.3 效果预览

板上小屏幕实时显示环境温度和湿度。

![image-20250424151219537](../../../../docs/pic/environment/image-20250424151219537.png)

## 1.4 接口介绍

### 1.4.1 uapi_i2c_master_read()


| **定义：**   | errcode_t uapi_i2c_master_read(i2c_bus_t bus, uint16_t dev_addr, i2c_data_t *data);                                                                                      |
| ------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **功能：**   | 主机接收来自目标I2C从机的数据，有两种方式，一种是手动切换方式，另外一种是自动切换模式，两种方式静态配置，手动切换方式一共有以下三种传输模式，但是不能在同一bus中同时使用 |
| **参数：**   | bus:I2C总线<br/>dev_addr：主机接收数据的目标从机地址  <br/>data:接收数据的数据指针                                                                                       |
| **返回值：** | ERRCODE_SUCC：成功    Other：失败                                                                                                                                        |
| **依赖：**   | include\driver\i2c.h                                                                                                                                                     |

### 1.4.2 uapi_i2c_master_write()


| 定义：       | errcode_t uapi_i2c_master_write(i2c_bus_t bus, uint16_t dev_addr, i2c_data_t *data);                                                                              |
| ------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **功能：**   | 将数据从主机写入到从机，有两种方式，一种是手动切换方式，另外一种是自动切换模式，两种方式静态配置，手动切换方式一共有以下三种传输模式，但是不能在同一bus中同时使用 |
| **参数：**   | bus:I2C总线<br/>dev_addr：主机接收数据的目标从机地址  <br/>data:发送数据的数据指针                                                                                |
| **返回值：** | ERRCODE_SUCC：成功    Other：失败                                                                                                                                 |
| **依赖：**   | include\driver\i2c.h                                                                                                                                              |

### 1.4.3 uapi_i2c_master_init()


| **定义：**   | errcode_t uapi_i2c_master_init(i2c_bus_t bus, uint32_t baudrate, uint8_t hscode);                                                         |
| ------------ | ----------------------------------------------------------------------------------------------------------------------------------------- |
| **功能：**   | 根据指定的参数初始化该i2c为主机                                                                                                           |
| **参数：**   | bus：I2C总线<br/>baudrate：i2c波特率  <br/>hscode：i2c高速模式主机码，每个主机有自己唯一的主机码，有效取值范围0~7，仅在高速模式下需要配置 |
| **返回值：** | ERRCODE_SUCC：成功    Other：失败                                                                                                         |
| **依赖：**   | include\driver\i2c.h                                                                                                                      |

### 1.4.4 uapi_pin_set_mode()


| **定义：**   | errcode_t uapi_pin_set_mode(pin_t pin, pin_mode_t mode); |
| ------------ | -------------------------------------------------------- |
| **功能：**   | 设置引脚复用模式                                         |
| **参数：**   | pin：io<br/>mode：复用模式                               |
| **返回值：** | ERRCODE_SUCC：成功    Other：失败                        |
| **依赖：**   | include\driver\pinctrl.h                                 |

### 1.4.5 ssd1306_SetCursor()


| **定义：**   | void ssd1306_SetCursor(uint8_t x, uint8_t y); |
| ------------ | --------------------------------------------- |
| **功能：**   | 设置字符串显示位置                            |
| **参数：**   | x：横坐标<br/>y：众坐标                       |
| **返回值：** | 无返回值                                      |
| **依赖：**   | oled\ssd1306.h                                |

### 1.4.6 ssd1306_DrawString()


| **定义：**   | char ssd1306_DrawString(char *str, FontDef Font, SSD1306_COLOR color); |
| ------------ | ---------------------------------------------------------------------- |
| **功能：**   | 设置输出的字符串                                                       |
| **参数：**   | str：要输出的字符串<br/>Font：字符串大小 <br/>color：颜色              |
| **返回值：** | ERRCODE_SUCC：成功    Other：失败                                      |
| **依赖：**   | oled\ssd1306.h                                                         |

### 1.4.7 ssd1306_UpdateScreen()


| **定义：**   | void ssd1306_UpdateScreen(void); |
| ------------ | -------------------------------- |
| **功能：**   | 在屏幕显示字符串                 |
| **参数：**   | 无                               |
| **返回值：** | 无                               |
| **依赖：**   | oled\ssd1306.h                   |

## 1.5 具体实现

步骤一：初始化I2C设备；

步骤二：I2C通信正常后，初始化OLED；

步骤三：通过数据手册中协议要求，发送数据

## 1.6 实验流程

- 步骤一：在xxx\src\application\samples\peripheral文件夹新建一个sample文件夹，在peripheral上右键选择“新建文件夹”，创建Sample文件夹，例如名称”environment“。

  ![image-70551992](../../../../docs/pic/beep/image-20240801170551992.png)
  
- 步骤二：将xxx\vendor\HiHope_NearLink_DK_WS63E_V03\environment文件里面内容拷贝到**步骤一创建的Sample文件夹中”environment“**。

  ![image-20240417175458664](../../../../docs/pic/environment/image-20240417175458664.png)
  
- 步骤三：在xxx\src\application\samples\peripheral\CMakeLists.txt文件中新增编译案例，具体如下图所示（如果不知道在哪个地方加的，可以在“set(SOURCES "${SOURCES}" PARENT_SCOPE)”上面一行添加）。

  ![image-20240802172456133](../../../../docs/pic/environment/image-20240802172456133.png)
  
- 步骤四：在xxx\src\application\samples\peripheral\Kconfig文件中新增编译案例，具体如下图所示（如果不知道在哪个地方加，可以在最后一行添加）。

  ![image-20240802172425982](../../../../docs/pic/environment/image-20240802172425982.png)
  
- 步骤五：点击如下图标，选择KConfig，具体选择路径“Application/Enable the Sample of peripheral”，在弹出框中选择“support ENVIRONMENT Sample”，点击Save，关闭弹窗。

  <img src="../../../../docs/pic/beep/image-20240801171406113.png" alt="image-20240801171406113" style="zoom: 67%;" /><img src="../../../../docs/pic/beep/image-20240205105234692-17119401758316.png" alt="image-20240205105234692" style="zoom: 67%;" /><img src="../../../../docs/pic/environment/image-20240417175558457.png" alt="image-20240417175558457" style="zoom:50%;" />
  
- 步骤六:找到Drivers->Drivers->I2C->I2C Configuration，按照图片勾选

  ![image-20250424142756195](../../../../docs/pic/environment/image-20250424142756195.png)
  
- 步骤七：点击“build”或者“rebuild”编译

  ![image-20240801112427220](../../../../docs/pic/tools/854badb5d2ae480c8827d80c5a993c45.png)
  
- 步骤八：编译完成如下图所示。

  ![image-20240801165456569](../../../../docs/pic/tools/image-20250307164622717.png)
  
- 步骤九：在HiSpark Studio工具中点击“工程配置”按钮，选择“程序加载”，传输方式选择“serial”，端口选择“comxxx”，com口在设备管理器中查看（如果找不到com口，请参考windows环境搭建）。

  ![image-20240801173929658](../../../../docs/pic/tools/image-20250317173145978.png)
  
- 步骤十：配置完成后，点击工具“程序加载”按钮烧录。

  ![image-20240801174117545](../../../../docs/pic/beep/image-20240801174117545.png)
  
- 步骤十一：出现“Connecting, please reset device...”字样时，复位开发板，等待烧录结束。

  ![image-20240801174230202](../../../../docs/pic/tools/image-20240801174230202.png)
  
- 步骤十二：“软件烧录成功后，按一下开发板的RESET按键复位开发板，板上小屏幕实时显示环境温度和湿度。

  ![image-20250424151225853](../../../../docs/pic/environment/image-20250424151225853.png)
