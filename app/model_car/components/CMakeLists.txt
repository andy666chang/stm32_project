
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ../../../components/ring_buf/ring_buf.c
    ../../../components/log/log.c
    ../../../components/shell/shell.c

    shell/shell.c
    shell/shell_sys.c
    shell/shell_led.c
    shell/shell_btn.c
    shell/shell_thro.c
    shell/shell_config.c
    shell/shell_led_bar.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)