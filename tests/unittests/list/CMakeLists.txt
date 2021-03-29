add_executable(testsuite_list tests_list.c)

set_target_properties(testsuite_list
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

target_link_libraries(testsuite_list PRIVATE
    ${CRITERION_LIBRARIES}
    common_options
    shell
    utils
)

add_test(NAME testsuite_list COMMAND testsuite_list)
