cmake_minimum_required(VERSION 3.12)

# Set the board type
set(PICO_BOARD pico_w)

# Import the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
# Name the project
set(ProjectName picoW-as-remote-debugger)
# Define the project
project(${ProjectName})

# Set the path to the FreeRTOS kernel and import the FreeRTOS kernel
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/debugprobe/freertos)
#include(debugprobe/FreeRTOS_Kernel_import.cmake) #debugprobe uses this
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake) # keijo uses this

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add debugprobe submodule as a subdirectory
#add_subdirectory(debugprobe)

# Add picoW-as-remote-debugger source files
add_executable(${ProjectName}
        src/main.cpp
)

# Include directories for your project
target_include_directories(${ProjectName} PRIVATE
        # ${CMAKE_CURRENT_SOURCE_DIR}/debugprobe/include
        # ${CMAKE_CURRENT_SOURCE_DIR}/debugprobe/freeRTOS/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src # so we know where to find FreeRTOSConfig.h
        # ${CMAKE_CURRENT_SOURCE_DIR}/debugprobe/CMSIS_5/CMSIS/DAP/Firmware/Include/
        # ${CMAKE_CURRENT_SOURCE_DIR}/debugprobe/CMSIS_5/CMSIS/Core/Include/ #removed cause i dont know what it does
        ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include/ # portmacro.h is here
        C:/pico/pico-sdk/pico-sdkV2.0/src/common/pico_base_headers/include # manually added pico.h
)

# Link debugprobe and its dependencies to your project
target_link_libraries(${ProjectName} PRIVATE
        debugprobe/FreeRTOS-Kernel
        debugprobe/FreeRTOS-Kernel-Heap1
        debugprobe/pico_stdlib
        debugprobe/pico_multicore
        debugprobe/tinyusb_device
        debugprobe/tinyusb_board
        src/FreeRTOSConfig
)

# Add any additional compile options (optional)
target_compile_options(${ProjectName} PRIVATE -Wall)

# Set binary type and add extra outputs
pico_set_binary_type(${ProjectName} copy_to_ram)
pico_add_extra_outputs(${ProjectName})
