# HiSpark Studio开发环境搭建

**备注：仅LiteOS 系统版本支持，Openharmony系统版本不支持windows环境搭建**

## 系统要求：

| 工具版本                                       | 下载链接                                                     |
| ---------------------------------------------- | ------------------------------------------------------------ |
| 支持操作系统:Windows10、Winndows11             |                                                              |
| Python：3.11.4版本                             | [python下载链接](https://www.python.org/ftp/python/3.11.4/python-3.11.4-amd64.exe) |
| cmake-3.20.5-py2.py3-none-win_amd64.whl        | [cmake下载链接](https://files.pythonhosted.org/packages/65/7f/80cf681cd376834b442af8af48e6f17b4197d20b7255aa2f76d8d93a9e44/cmake-3.20.5-py2.py3-none-win_amd64.whl) |
| kconfiglib-14.1.0-py2.py3-none-any.whl         | [kconfiglib下载链接](https://files.pythonhosted.org/packages/8a/f1/d98a89231e779b079b977590efcc31249d959c8f1d4b5858cad69695ff9c/kconfiglib-14.1.0-py2.py3-none-any.whl) |
| pycparser-2.21-py2.py3-none-any.whl            | [pycparser下载链接](https://files.pythonhosted.org/packages/62/d5/5f610ebe421e85889f2e55e33b7f9a6795bd982198517d912eb1c76e1a53/pycparser-2.21-py2.py3-none-any.whl) |
| windows_curses-2.3.3-cp311-cp311-win_amd64.whl | [windows_curses下载链接](https://files.pythonhosted.org/packages/18/1b/e06eb41dad1c74f0d3124218084f258f73a5e76c67112da0ba174162670f/windows_curses-2.3.3-cp311-cp311-win_amd64.whl) |

## HiSpark Studio下载

- 登录上海海思开发者网站获取IDE工具，需要注册账号下载，下载最新版本即可，下图只作为参考。

  ![image.png](../vendor/HH-D03/doc/media/tools/1.png)

- 如果无法登录开发者网站下载，可以访问此链接下载（建议在海思开发者网站下载）：[https://hispark-obs.obs.cn-east-3.myhuaweicloud.com/HiSparkStudio.exe](https://gitee.com/link?target=https%3A%2F%2Fhispark-obs.obs.cn-east-3.myhuaweicloud.com%2FHiSparkStudio.exe)

## HiSpark Studio安装

- 下载完成后，双击"HiSparkStudio1.0.0.11.exe"安装。下载最新版本即可，下图只作为参考。

  ![image.png](../vendor/HH-D03/doc/media/tools/2.png)

- 安装界面如下，选择“我同意此协议”，点击“下一步”。

  ![image.png](../vendor/HH-D03/doc/media/tools/3.png)

- 根据用户自身磁盘空间大小，选择对应的磁盘进行安装，选择完成后，点击下一步。（**注意如果你的用户名带中文目录，请安装到D盘、E盘等其他目录**，**这里以D盘为例**）

  ![image.png](../vendor/HH-D03/doc/media/tools/4.png)

- 根据用户自身需要勾选附加任务，默认全部勾选，选择完成后，点击下一步。

  ![image.png](../vendor/HH-D03/doc/media/tools/5.png)

- 点击“安装”，进行工具的安装。

  ![image-20250306181441344](../vendor/HH-D03/doc/media/tools/6.png)

- 安装过程如下，等待安装完成。

  ![image.png](../vendor/HH-D03/doc/media/tools/7.png)

- 如果在安装过程中弹出“python 3.11.4”会自动弹出“python”安装提示，点击“取消”即可

  ![image-20250306181829205](../vendor/HH-D03/doc/media/tools/10.png)

- 出现如下界面代表安装完成，点击完成即可。

  ![image.png](../vendor/HH-D03/doc/media/tools/8.png)

- HiSpark studio打开主页界面如下。

  ![image.png](../vendor/HH-D03/doc/media/tools/9.png)

##  环境变量添加

- 工具安装完成后，在电脑系统环境变量中，添加环境变量（这里以win10为例，如果是win11，可以在百度搜索如何添加环境变量），在此电脑鼠标“右键”，点击“属性”

  ![image-20250307143252345](../vendor/HH-D03/doc/media/tools/image-20250307143252345.png)

- 弹出系统属性框，点击“环境变量”

  ![image-20250307143539205](../vendor/HH-D03/doc/media/tools/image-20250307143539205.png)

- 在系统环境变量，选择“Path”，双击进入

  ![image-20250307143625753](../vendor/HH-D03/doc/media/tools/image-20250307143625753.png)

- 在编辑环境变量中添加“xxx\HiSpark Studio\tools\Windows\ninja”、“xxx\HiSpark Studio\tools\Windows\gn”、“xxx\HiSpark Studio\tools\Windows\cc_riscv32_musl_fp_win\bin”在几个环境变量（xxx代表HiSpark Studio安装目录），添加完成后，点击“确定”。

  ![image-20250307142741299](../vendor/HH-D03/doc/media/tools/image-20250307142741299.png)

  ![image-20250307145245437](../vendor/HH-D03/doc/media/tools/image-20250307145245437.png)

  ![image-20250307145204942](../vendor/HH-D03/doc/media/tools/image-20250307145204942.png)

  ![image-20250307144012214](../vendor/HH-D03/doc/media/tools/image-20250307144012214.png)

- 环境变量安装完成后，测试是否添加成， 在“命令行窗口分别输入“ninja --version”、“gn -version”、“riscv32-linux-musl-gcc -v”，出现如下图所示版本号代表成功

  ![image-20250307150120146](../vendor/HH-D03/doc/media/tools/image-20250307150120146.png)

  ## python下载及安装

- 环境变量配置成功后，下载python（版本：3.11.4），[python下载地址：https://www.python.org/ftp/python/3.11.4/python-3.11.4-amd64.exe](https://www.python.org/ftp/python/3.11.4/python-3.11.4-amd64.exe)

  ![image-20250307095229583](../vendor/HH-D03/doc/media/tools/11.png)

- 下载完成后，点击安装，安装界面勾选“ADD python.exe to PATH”，选择“Customize installation”安装

  ![image-20250307095303931](../vendor/HH-D03/doc/media/tools/12.png)

- 点击“next”即可

  ![image-20250307095731813](../vendor/HH-D03/doc/media/tools/13.png)

- 修改安装路径，如果电脑用户名不带中文可以默认安装，如果电脑用户名带中文，请安装其他磁盘（这里以D盘为例），选择好路径以后，点击“install”（**注意：安装目录不要带中文路径**）

  ![image-20250307095922285](../vendor/HH-D03/doc/media/tools/14.png)

- 等待安装完成即可

  ![image-20250307100139436](../vendor/HH-D03/doc/media/tools/15.png)

- 安装完成后，点击“close”即可

  ![image-20250307100526042](../vendor/HH-D03/doc/media/tools/16.png)

- 打开“命令行窗口”，输入“python”，显示3.11.4即成功

  ![image-20250306181922046](../vendor/HH-D03/doc/media/tools/17.png)

## 编译插件安装

- 下载 [kconfiglib下载链接](https://files.pythonhosted.org/packages/8a/f1/d98a89231e779b079b977590efcc31249d959c8f1d4b5858cad69695ff9c/kconfiglib-14.1.0-py2.py3-none-any.whl) 

- 下载插件 [cmake下载链接](https://files.pythonhosted.org/packages/65/7f/80cf681cd376834b442af8af48e6f17b4197d20b7255aa2f76d8d93a9e44/cmake-3.20.5-py2.py3-none-win_amd64.whl)

- 下载插件 [pycparser下载链接](https://files.pythonhosted.org/packages/62/d5/5f610ebe421e85889f2e55e33b7f9a6795bd982198517d912eb1c76e1a53/pycparser-2.21-py2.py3-none-any.whl) 

- 下载插件 [windows_curses下载链接](https://files.pythonhosted.org/packages/18/1b/e06eb41dad1c74f0d3124218084f258f73a5e76c67112da0ba174162670f/windows_curses-2.3.3-cp311-cp311-win_amd64.whl) 

- 将kconfiglib、cmake、pycparser、windows_curses等文件存放在同一个目录下（**任意目录即可**），在目录文件夹上放输入“cmd”

  ![image.png](../vendor/HH-D03/doc/media/tools/18.png)
  
- 在“命令行窗口”输入pip install windows_curses-2.3.3-cp311-cp311-win_amd64.whl

  ![image-20250307163505547](../vendor/HH-D03/doc/media/tools/image-20250307163505547.png)

- 在“命令行窗口”输入pip install cmake-3.20.5-py2.py3-none-win_amd64.whl

  ![image-20250307163428170](../vendor/HH-D03/doc/media/tools/image-20250307163428170.png)
  
- 在“命令行窗口”输入pip install kconfiglib-14.1.0-py2.py3-none-any.whl

  ![image-20250307163648419](../vendor/HH-D03/doc/media/tools/image-20250307163648419.png)
  
- 在“命令行窗口”输入pip install pycparser-2.21-py2.py3-none-any.whl

  ![image-20250307163706951](../vendor/HH-D03/doc/media/tools/image-20250307163706951.png)

