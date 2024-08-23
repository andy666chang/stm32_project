cmake_minimum_required(VERSION 3.22)

project(nuvoton)
add_library(nuvoton INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(nuvoton INTERFACE 
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(nuvoton INTERFACE
    ../../../../sdk/M480BSP/Library/Device/Nuvoton/M480/include
    ../../../../sdk/M480BSP/Library/StdDriver/inc
    ../../../../sdk/M480BSP/Library/CMSIS/Include
)

target_sources(nuvoton INTERFACE
    # ../../../../sdk/M480BSP/Library/StdDriver/src/acmp.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/bpwm.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/can.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/ccap.c
    ../../../../sdk/M480BSP/Library/StdDriver/src/clk.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/crc.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/crypto.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/dac.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/eadc.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/ebi.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/ecap.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/emac.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/epwm.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/fmc.c
    ../../../../sdk/M480BSP/Library/StdDriver/src/gpio.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/hsusbd.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/i2c.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/i2s.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/pdma.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/qei.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/qspi.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/rtc.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/sc.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/scuart.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/sdh.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/spi.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/spim.c
    ../../../../sdk/M480BSP/Library/StdDriver/src/sys.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/timer.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/timer_pwm.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/trng.c
    ../../../../sdk/M480BSP/Library/StdDriver/src/uart.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/usbd.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/usci_i2c.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/usci_spi.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/usci_uart.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/wdt.c
    # ../../../../sdk/M480BSP/Library/StdDriver/src/wwdt.c
)

target_link_directories(nuvoton INTERFACE
)

target_link_libraries(nuvoton INTERFACE
)

# Validate that Nuvoton code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


