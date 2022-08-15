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
    test_cosem_register
    test_cosem_register.c
    ${SRC}/dlms.c
    ${SRC}/cosem.c
)

target_link_libraries(test_cosem_register PRIVATE m)

enable_testing()

add_test(NAME test_dlms_parse COMMAND $<TARGET_FILE:test_dlms_parse>)
add_test(NAME test_cosem_register COMMAND $<TARGET_FILE:test_cosem_register>)