#===============================================================================
# @brief    cmake file
# Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2022. All rights reserved.
#===============================================================================
include("${CMAKE_CURRENT_SOURCE_DIR}/bs2x_sample_config.cmake")
add_subdirectory_if_exist(porting)
add_subdirectory_if_exist(rom)
add_subdirectory_if_exist(boot)
add_subdirectory_if_exist(test_suite_stub)
set(COMPONENT_NAME "chip_porting")

set(SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/arch/src/interrupt.c
${CMAKE_CURRENT_SOURCE_DIR}/arch/src/interrupt_handler.c
${CMAKE_CURRENT_SOURCE_DIR}/arch/src/mpu.c
${CMAKE_CURRENT_SOURCE_DIR}/arch/src/vectors.s
${CMAKE_CURRENT_SOURCE_DIR}/arch/src/patch.c
${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt_adapter.c
${CMAKE_CURRENT_SOURCE_DIR}/interrupt/vectors.c
)

set(PUBLIC_HEADER
${CMAKE_CURRENT_SOURCE_DIR}/arch/include
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/boot
)

if(DEFINED OS AND NOT ${OS} STREQUAL "non-os")
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/src/oam_trace.c)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt_os_adapter.c)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/idle_config.c)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/tick_timer.c)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/memory_info.c)
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/os_dfx.c)
list(APPEND PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/liteos)
endif()

if(${CHIP_FAMILY} MATCHES "bs2x")
    set(PUBLIC_DEFINES
        CHIP_BS2X=1
        CHIP_BS2X_VERSION="${CHIP} 1.0.15"
    )
else()
    set(PUBLIC_DEFINES
        CHIP_BS2X=0
    )
endif()

set(WHOLE_LINK
true
)

set(MAIN_COMPONENT
false
)

build_component()

set(COMPONENT_NAME "mini_chip_porting")

set(SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt_adapter.c
)

set(PUBLIC_HEADER
${CMAKE_CURRENT_SOURCE_DIR}/arch/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/boot
)

if(DEFINED OS AND NOT ${OS} STREQUAL "non-os")
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/src/oam_trace.c)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt_os_adapter.c)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/idle_config.c)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/tick_timer.c)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/memory_info.c)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/os_dfx.c)
    list(APPEND PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/liteos)
endif()

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

set(COMPONENT_NAME "clocks_porting")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/clocks.c
)

if(NOT ((${APPLICATION} STREQUAL "rom") OR (${APPLICATION} STREQUAL "loaderboot") OR
        (${APPLICATION} STREQUAL "flashboot") OR (${APPLICATION} STREQUAL "romboot")))
set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/clock_calibration.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal_32k_clock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/clocks_switch.c
)
endif()

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
    SUPPORT_CLOCKS_CORE
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

set(COMPONENT_NAME "pmu_porting")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
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

set(COMPONENT_NAME "main_init_porting")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main_init/main_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main_init/app_os_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main_init/main_test.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/main_init
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
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

install_sdk("${ROOT_DIR}/interim_binary/${CHIP}/bin/boot_bin" "*.bin")

if(NOT ${LOADERBOOT_CFG} EQUAL "")
install_sdk("${ROOT_DIR}/interim_binary/${CHIP}/bin/boot_bin/${LOADERBOOT_CFG}" "*.bin")
endif()

if(NOT ${FLASHBOOT_CFG} EQUAL "")
install_sdk("${ROOT_DIR}/interim_binary/${CHIP}/bin/boot_bin/${FLASHBOOT_CFG}" "*.bin")
endif()

if(NOT ${SEC_BOOT} EQUAL "")
install_sdk("${ROOT_DIR}/interim_binary/${CHIP}/bin/boot_bin/${SEC_BOOT}" "*.bin")
endif()

set(COMPONENT_NAME "sfc_patch")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/src/sfc_patch.c
)

set(PUBLIC_HEADER
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