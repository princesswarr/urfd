cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico_w)

include(debugprobe/pico_sdk_import.cmake)

# this will use our FreeRTOS fork
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS/FreeRTOS/source)
# this will use the debugprobe fork of FreeRTOS
#set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/debugprobe/freertos)

include(debugprobe/FreeRTOS_Kernel_import.cmake)

set(ProjectName picoW-as-remote-debugger)
project(${ProjectName})

pico_sdk_init()

if (${PICO_SDK_VERSION_MAJOR} LESS 2)
    message(SEND_ERROR "Version 2 of the Pico SDK is required to compile this project. Please update your installation at ${PICO_SDK_PATH}")
endif ()

add_executable(${ProjectName}
        debugprobe/src/probe_config.c
        src/main.cpp
        debugprobe/src/usb_descriptors.c
        debugprobe/src/probe.c
        debugprobe/src/cdc_uart.c
        debugprobe/src/get_serial.c
        debugprobe/src/sw_dp_pio.c
        debugprobe/src/tusb_edpt_handler.c
        src/tcp_server.cpp
        src/tcp_server.h
        src/tcp_connections.cpp
        src/tcp_connections.h
        src/tcp_packet_processor.cpp
        src/tcp_packet_processor.h
)

target_sources(${ProjectName} PRIVATE
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
        debugprobe/src/
)

target_compile_options(${ProjectName} PRIVATE -Wall)

pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/debugprobe/src/probe.pio)
pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/debugprobe/src/probe_oen.pio)

target_include_directories(${ProjectName} PRIVATE src)

target_compile_definitions (${ProjectName} PRIVATE
        PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1
)

option (DEBUG_ON_PICO "Compile firmware for the Pico instead of Debug Probe" OFF)
if (DEBUG_ON_PICO)
    target_compile_definitions (${ProjectName} PRIVATE
            DEBUG_ON_PICO=1
    )
    if (PICO_BOARD STREQUAL "pico")
        set_target_properties(${ProjectName} PROPERTIES
                OUTPUT_NAME "debugprobe_on_pico"
        )
    elseif (PICO_BOARD STREQUAL "pico2")
        set_target_properties(${ProjectName} PROPERTIES
                OUTPUT_NAME "debugprobe_on_pico2"
        )
    else ()
        error("Unsupported board ${PICO_BOARD}" PICO_BOARD)
    endif ()
endif ()


target_link_libraries(${ProjectName} PRIVATE
        pico_multicore
        pico_stdlib
        pico_unique_id
        tinyusb_device
        tinyusb_board
        hardware_pio
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap1
)

pico_set_binary_type(${ProjectName} copy_to_ram)

pico_add_extra_outputs(${ProjectName})
