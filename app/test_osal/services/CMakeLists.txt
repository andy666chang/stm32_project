
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    led/service.c
    uart/service.c
    shell/service.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)
