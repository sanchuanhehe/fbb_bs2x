#===============================================================================
# @brief    cmake make file
# Copyright (c) 2023 HiSilicon (Shanghai) Technologies Co., Ltd.
#===============================================================================

set(COMPONENT_NAME "dfx_file_operation")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dfx_file_operation.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
