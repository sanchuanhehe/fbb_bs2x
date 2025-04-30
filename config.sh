#!/bin/bash

# 设置非交互模式以避免时间区选择等提示
export DEBIAN_FRONTEND=noninteractive

# 更新系统并安装必要的工具和依赖
sudo apt-get update
sudo apt-get install -y \
    cmake \
    python3 \
    python3-pip \
    python3-setuptools \
    wget \
    build-essential \
    sudo \
    curl \
    git \
    bash \
    ninja-build \
    unzip \
    doxygen \
    clang-format

# 配置shell为bash
echo "dash dash/sh boolean false" | sudo debconf-set-selections
sudo dpkg-reconfigure -p critical dash

# 从 LLVM 官方下载最新的 clangd
CLANGD_VERSION=18.1.3
wget https://github.com/clangd/clangd/releases/download/$CLANGD_VERSION/clangd-linux-$CLANGD_VERSION.zip
unzip clangd-linux-$CLANGD_VERSION.zip
sudo cp -r clangd_$CLANGD_VERSION/bin/* /usr/local/bin/
sudo cp -r clangd_$CLANGD_VERSION/lib/* /usr/local/lib/
rm -rf clangd-linux-$CLANGD_VERSION.zip clangd_$CLANGD_VERSION

# 安装kconfiglib，版本大于14.1.0
pip3 install "kconfiglib>=14.1.0"

# 安装其他Python依赖包，版本大于2.21
pip3 install "pycparser>=2.21"

# 安装PYYAML
pip3 install pyyaml

# 设置环境变量
export LC_ALL=C.UTF-8

# 添加环境变量 PATH
export PATH="/sdk/tools/bin/compiler/riscv/cc_riscv32_musl_b090/cc_riscv32_musl_fp/bin:${PATH}"

echo "环境初始化完成！"