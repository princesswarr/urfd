cmake_minimum_required(VERSION 3.12)

#Set board type
set(PICO_BOARD pico_w)

# Name project
SET(ProjectName picoW-as-remote-debugger)

# Set any variables required for importing libraries
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-KernelV10.6.2)

# Import those libraries
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName})

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(debugprobe)

# Define the executable
add_executable(${ProjectName} src/main.cpp)

# Link libraries (including FreeRTOS and debugprobe)
target_link_libraries(${ProjectName}
        pico_stdlib
        FreeRTOS-Kernel
        debugprobe  # Link the debugprobe library
)

# Add any additional flags or options here if needed
pico_add_extra_outputs(${ProjectName})
