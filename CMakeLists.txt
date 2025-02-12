cmake_minimum_required(VERSION 3.12)

# Set board type
set(PICO_BOARD pico_w)

# Name project
set(ProjectName picoW-as-remote-debugger)

# this will use our FreeRTOS fork
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS/FreeRTOS/source)
# this will use the debugprobe fork of FreeRTOS
#set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/debugprobe/freertos)

# Import those libraries
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName})

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (${PICO_SDK_VERSION_MAJOR} LESS 2)
    message(SEND_ERROR "Version 2 of the Pico SDK is required to compile this project. Please update your installation at ${PICO_SDK_PATH}")
endif ()

add_executable(${ProjectName}
        debugprobe/src/probe_config.c
        debugprobe/src/usb_descriptors.c
        debugprobe/src/probe.c
        debugprobe/src/cdc_uart.c
        debugprobe/src/get_serial.c
        debugprobe/src/sw_dp_pio.c
        debugprobe/src/tusb_edpt_handler.c
        src/ipstack/IPStack.cpp
        src/tcp_server.cpp
        src/main.cpp
        debugprobe/CMSIS_5/CMSIS/DAP/Firmware/Source/DAP.c
        debugprobe/CMSIS_5/CMSIS/DAP/Firmware/Source/JTAG_DP.c
        debugprobe/CMSIS_5/CMSIS/DAP/Firmware/Source/DAP_vendor.c
        debugprobe/CMSIS_5/CMSIS/DAP/Firmware/Source/SWO.c
        #debugprobe/CMSIS_5/CMSIS/DAP/Firmware/Source/SW_DP.c
)

target_include_directories(${ProjectName} PRIVATE
        debugprobe/CMSIS_5/CMSIS/DAP/Firmware/Include/
        debugprobe/CMSIS_5/CMSIS/Core/Include/
        debugprobe/include/
        debugprobe/src
        src
        src/ipstack
)

target_compile_options(${ProjectName} PRIVATE -Wall)

pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/debugprobe/src/probe.pio)
pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/debugprobe/src/probe_oen.pio)

target_compile_definitions(${ProjectName} PRIVATE
        PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1
)

option (DEBUG_ON_PICO "Compile firmware for the Pico instead of Debug Probe" ON)
if (DEBUG_ON_PICO)
    target_compile_definitions (${ProjectName} PRIVATE
            DEBUG_ON_PICO=1
    )
    if (PICO_BOARD STREQUAL "pico_w")
        set_target_properties(${ProjectName} PROPERTIES
                OUTPUT_NAME "debugprobe_on_pico"
        )
    elseif (PICO_BOARD STREQUAL "pico2")
        set_target_properties(${ProjectName} PROPERTIES
                OUTPUT_NAME "debugprobe_on_pico2"
        )
    else ()
        message(SEND_ERROR "Unsupported board ${PICO_BOARD}")
    endif ()
endif ()

target_link_libraries(${ProjectName}
        pico_stdlib
        FreeRTOS-Kernel-Heap4 # used in keijos version
        pico_cyw43_arch_lwip_sys_freertos
        pico_mbedtls
        pico_lwip_mbedtls

        pico_multicore
        pico_unique_id
        tinyusb_device
        tinyusb_board
        hardware_pio
        FreeRTOS-Kernel
        #FreeRTOS-Kernel-Heap1 # used in debugprobe version
)

#pico_set_binary_type(${ProjectName} copy_to_ram)

pico_add_extra_outputs(${ProjectName})
