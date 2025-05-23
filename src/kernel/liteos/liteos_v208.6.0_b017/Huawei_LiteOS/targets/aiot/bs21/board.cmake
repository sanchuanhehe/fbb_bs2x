# C sources
set(LOCAL_SRCS_y
${CMAKE_CURRENT_SOURCE_DIR}/${LOSCFG_FAMILY}/${LITEOS_PLATFORM}/os_adapt/os_adapt.c
${CMAKE_CURRENT_SOURCE_DIR}/${LOSCFG_FAMILY}/${LITEOS_PLATFORM}/board.c
${CMAKE_CURRENT_SOURCE_DIR}/${LOSCFG_FAMILY}/${LITEOS_PLATFORM}/msg_chl.c
# ${CMAKE_CURRENT_SOURCE_DIR}/${LOSCFG_FAMILY}/${LITEOS_PLATFORM}/runtime_monitor.c
${CMAKE_CURRENT_SOURCE_DIR}/${LOSCFG_FAMILY}/${LITEOS_PLATFORM}/los_irmalloc.c
${CMAKE_CURRENT_SOURCE_DIR}/${LOSCFG_FAMILY}/${LITEOS_PLATFORM}/uart.c
)

list(APPEND MODULE_INCLUDE_PUB ${CMAKE_CURRENT_SOURCE_DIR}/${LOSCFG_FAMILY}/${LITEOS_PLATFORM}/include)
