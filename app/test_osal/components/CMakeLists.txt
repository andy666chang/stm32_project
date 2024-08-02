
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ../../../components/osal/src/osal_clock.c
    ../../../components/osal/src/osal_memory.c
    ../../../components/osal/src/osal_msg.c
    ../../../components/osal/src/osal_tasks.c
    ../../../components/osal/src/osal_timers.c
    ../../../components/osal/src/osal.c
    ../../../components/ring_buf/ring_buf.c
    ../../../components/log/log.c
    ../../../components/shell/shell.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ../../../components/osal/inc
    ../../../components/ring_buf
    osal
)