#===============================================================================
# @brief    cmake file
# Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2024. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "partition_porting")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/partition_porting.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ROOT_DIR}/middleware/chips/${CHIP}/partition/include
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

set(COMPONENT_CCFLAGS
"-Wno-incompatible-pointer-types"
)

set(WHOLE_LINK
true
)

build_component()
