cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME TEMiWCZ_BLDC_SW)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Drivers Midllewares
set(MotorControl_Src
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Src/pid_regulator.c
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Src/sto_pll_speed_pos_fdbk.c
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Src/speed_torq_ctrl.c
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Src/revup_ctrl.c
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Src/pid_regulator.c
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Src/open_loop.c
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Src/ramp_ext_mngr.c

    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/G4xx/Src/r3_1_g4xx_pwm_curr_fdbk.c

)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_SOURCE_DIR}/Core/Src/GPIO.c
    ${CMAKE_SOURCE_DIR}/Core/Src/ADC.c
    ${CMAKE_SOURCE_DIR}/Core/Src/DMA.c
    ${CMAKE_SOURCE_DIR}/Core/Src/USB.c
    ${CMAKE_SOURCE_DIR}/Core/Src/TIM.c
    ${CMAKE_SOURCE_DIR}/Core/Src/drv8323.c
    ${CMAKE_SOURCE_DIR}/Core/Src/Scheduler.c
    ${CMAKE_SOURCE_DIR}/Core/Src/MC.c
    ${CMAKE_SOURCE_DIR}/Core/Src/mc_config.c
    ${CMAKE_SOURCE_DIR}/Core/Src/mc_config_common.c
    ${CMAKE_SOURCE_DIR}/Core/Src/mc_configuration_registers.c
    ${CMAKE_SOURCE_DIR}/Core/Src/mc_parameters.c
    ${CMAKE_SOURCE_DIR}/Core/Src/pwm_common.c
    ${CMAKE_SOURCE_DIR}/Core/Src/pwm_curr_fdbk.c
    ${CMAKE_SOURCE_DIR}/Core/Src/FOC.c
)

# Link directories setup
set(MX_LINK_DIRS
    ${CMAKE_SOURCE_DIR}/MCSDK/libHSO
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    # Add user defined include paths
    ${CMAKE_SOURCE_DIR}/Core/Inc/drv8323.h
)

# Create MotorControl static library
add_library(MotorControl OBJECT)
target_sources(MotorControl PRIVATE ${MotorControl_Src})
target_link_libraries(MotorControl PUBLIC stm32cubemx)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
	:libHSO.a 
	:libImpedCorr.a 
	:libPolPulse.a 
	:libRsEst.a
    MotorControl
    # Add user defined libraries
)

target_include_directories(MotorControl PUBLIC
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/Any/Inc
    ${CMAKE_SOURCE_DIR}/MCSDK/MCLib/G4xx/Inc
    ${CMAKE_SOURCE_DIR}/MCSDK/libHSO/
)
# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_DIRS})