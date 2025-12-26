cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_FULL_LL_DRIVER 
	USE_HAL_DRIVER 
	STM32G031xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
    ../../../../sdk/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include
    ../../../../sdk/STM32CubeG0/Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/gpio.c
    ../../Core/Src/crc.c
    ../../Core/Src/dma.c
    ../../Core/Src/tim.c
    ../../Core/Src/usart.c
    ../../Core/Src/stm32g0xx_it.c
    ../../Core/Src/stm32g0xx_hal_msp.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_utils.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_exti.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_gpio.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_crc.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_crc_ex.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc_ex.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_rcc.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash_ex.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma_ex.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr_ex.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_exti.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim_ex.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart.c
    ../../../../sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart_ex.c
    ../../Core/Src/system_stm32g0xx.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32g031xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


