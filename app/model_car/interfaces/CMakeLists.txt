
FILE(GLOB interface_src *.c)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${interface_src}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)
