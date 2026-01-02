
set (COMPONENT_PATH "../../../components")

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${COMPONENT_PATH}/ring_buf/ring_buf.c
    ${COMPONENT_PATH}/log/log.c
    ${COMPONENT_PATH}/shell/shell.c

    shell/shell_sys.c
    shell/shell_led.c
    shell/shell_btn.c
    shell/shell_thro.c
    shell/shell_config.c
    shell/shell_led_bar.c
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/${COMPONENT_PATH}/shell/shell.ld
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)