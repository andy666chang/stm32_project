
add_subdirectory(../../../../components/mcuboot/boot/bootutil mcuboot)

target_include_directories(bootutil
    PUBLIC
        ./
    PRIVATE
)

target_sources(bootutil
    PRIVATE
        ./mcu_port.c
)


# Add linked libraries
target_link_libraries(bootutil
    tinycrypt
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    bootutil
)
