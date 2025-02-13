set(EXE_SOURCES
    "main.c")

add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})

target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${TRANSLATELIB})

