cmake_minimum_required(VERSION 3.14)
project(dlms C)

set(SRC ../src)
include_directories(${SRC} .)

add_executable(
    test_dlms_parse
    test_dlms_parse.c
    ${SRC}/dlms.c
)

add_executable(
    test_cosem_time
    test_cosem_time.c
    ${SRC}/cosem_time.c
)

add_executable(
    test_step_over
    test_step_over.c
    ${SRC}/dlms.c
)

add_executable(
    test_cosem_register
    test_cosem_register.c
    ${SRC}/dlms.c
    ${SRC}/cosem.c
)

add_executable(
    test_kamstrup_data
    test_kamstrup_data.c
    ${SRC}/dlms.c
)

target_link_libraries(test_cosem_register PRIVATE m)

enable_testing()

add_test(NAME test_dlms_parse COMMAND $<TARGET_FILE:test_dlms_parse>)
add_test(NAME test_cosem_register COMMAND $<TARGET_FILE:test_cosem_register>)
add_test(NAME test_cosem_time COMMAND $<TARGET_FILE:test_cosem_time>)