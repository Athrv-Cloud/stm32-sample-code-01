cmake_minimum_required(VERSION 3.15)

# Project setup
project(stm32_project C ASM)

# Avoid linking standard library system calls
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set cross-compilation environment
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Define the toolchain with explicit paths
set(TOOLCHAIN arm-none-eabi)
set(TOOLCHAIN_PATH "/usr/bin") # Replace with the correct path to your toolchain

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN}-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN}-g++)
set(CMAKE_AR ${TOOLCHAIN_PATH}/${TOOLCHAIN}-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/${TOOLCHAIN}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}/${TOOLCHAIN}-objdump)
set(CMAKE_SIZE ${TOOLCHAIN_PATH}/${TOOLCHAIN}-size)

# MCU-specific flags
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Compiler options
add_compile_options(
    -DSTM32F446RE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    -O2
    -g3
    -ffunction-sections
    -fdata-sections
    -fno-common
    -fno-builtin
    -Wno-unused-but-set-variable
    -Wno-sign-compare
)

# Linker options
add_link_options(
    ${CPU_PARAMETERS}
    -Wl,--gc-sections
    -specs=nano.specs
    -specs=nosys.specs
    -T${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld
    -Wl,--start-group
    -lc
    -lm
    -Wl,--end-group
)

# Include directories
include_directories(
    Core/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)

# Add executable
add_executable(${PROJECT_NAME}
    Core/Src/main.c
    Core/Src/stm32f4xx_it.c
    Core/Src/stm32f4xx_hal_msp.c
    Core/Src/system_stm32f4xx.c
    Core/Src/syscalls.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c  # Added for RCCEx functions
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c  # Added for NVIC and SYSTICK functions
    Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/startup_stm32f446xx.s
)

# Generate HEX and BIN files after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
)

# Print size after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}
)

