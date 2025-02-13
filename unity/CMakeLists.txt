set(TEST_FILES
    "test.c"
    "unity_internals.h"
    "unity.c"
    "unity.h")

add_executable(${TEST_EXECUTABLE} ${TEST_FILES})

target_link_libraries(${TEST_EXECUTABLE} PUBLIC ${TRANSLATELIB})
