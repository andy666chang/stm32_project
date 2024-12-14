cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32G031xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/gpio.c
    ../../Core/Src/usart.c
    ../../Core/Src/stm32g0xx_it.c
    ../../Core/Src/stm32g0xx_hal_msp.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim_ex.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart_ex.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc_ex.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_rcc.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash_ex.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma_ex.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr_ex.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c
    D:/Andy/Desktop/stm_project/sdk/STM32CubeG0/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_exti.c
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


