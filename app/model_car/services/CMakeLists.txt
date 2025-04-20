
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    button/service.c
    throttle/service.c
    shell/service.c

    throttle/config.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add extra linker
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/services.ld
)
