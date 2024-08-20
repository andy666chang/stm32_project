cmake_minimum_required(VERSION 3.22)

project(nuvoton)
add_library(nuvoton INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(nuvoton INTERFACE 
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(nuvoton INTERFACE
    ../../../../sdk/M031BSP/Library/Device/Nuvoton/M031/include
    ../../../../sdk/M031BSP/Library/StdDriver/inc
    ../../../../sdk/M031BSP/Library/CMSIS/Include
)

target_sources(nuvoton INTERFACE
    # ../../../../sdk/M031BSP/Library/StdDriver/src/acmp.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/crc.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/i2c.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/retarget.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/timer.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/usci_spi.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/adc.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/ebi.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/pdma.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/rtc.c
    ../../../../sdk/M031BSP/Library/StdDriver/src/uart.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/usci_uart.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/bpwm.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/fmc.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/pwm.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/spi.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/usbd.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/wdt.c
    ../../../../sdk/M031BSP/Library/StdDriver/src/clk.c
    ../../../../sdk/M031BSP/Library/StdDriver/src/gpio.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/qspi.c
    ../../../../sdk/M031BSP/Library/StdDriver/src/sys.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/usci_i2c.c
    # ../../../../sdk/M031BSP/Library/StdDriver/src/wwdt.c
)

target_link_directories(nuvoton INTERFACE
)

target_link_libraries(nuvoton INTERFACE
)

# Validate that Nuvoton code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


