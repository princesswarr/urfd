cmake_minimum_required(VERSION 3.12)

#Set board type
set(PICO_BOARD pico_w)

include(debugprobe/pico_sdk_import.cmake)

# Set any variables required for importing libraries
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/debugprobe/freertos)
include(debugprobe/FreeRTOS_Kernel_import.cmake)

# Import those libraries
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Name project
SET(ProjectName picoW-as-remote-debugger)
# Define project
project(${ProjectName})

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# add debugprobe executables
add_executable(${ProjectName}
        debugprobe/src/probe_config.c
        debugprobe/src/usb_descriptors.c
        debugprobe/src/probe.c
        debugprobe/src/cdc_uart.c
        debugprobe/src/get_serial.c
        debugprobe/src/sw_dp_pio.c
        debugprobe/src/tusb_edpt_handler.c
        src/main.cpp
)

#
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
)

pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/debugprobe/src/probe.pio)
pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/debugprobe/src/probe_oen.pio)

target_include_directories(${ProjectName} PRIVATE src)

target_compile_definitions (${ProjectName} PRIVATE
        PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1
)

option (DEBUG_ON_PICO "Compile firmware for the Pico instead of Debug Probe" OFF)
if (DEBUG_ON_PICO)
    target_compile_definitions (debugprobe PRIVATE
            DEBUG_ON_PICO=1
    )
    set_target_properties(debugprobe PROPERTIES
            OUTPUT_NAME "debugprobe_on_pico"
    )
endif ()

# Link libraries (including FreeRTOS and debugprobe)
target_link_libraries(${ProjectName} PRIVATE
        debugprobe/pico_multicore
        debugprobe/pico_stdlib
        debugprobe/pico_unique_id
        debugprobe/tinyusb_device
        debugprobe/tinyusb_board
        debugprobe/hardware_pio
        debugprobe/FreeRTOS-Kernel
        debugprobe/FreeRTOS-Kernel-Heap1
)

pico_set_binary_type(${ProjectName} copy_to_ram)

# Add any additional flags or options here if needed
pico_add_extra_outputs(${ProjectName})


