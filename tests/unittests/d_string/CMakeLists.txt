add_executable(testsuite_d_string tests_d_string.c)

set_target_properties(testsuite_d_string
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

target_link_libraries(testsuite_d_string PRIVATE
    ${CRITERION_LIBRARIES}
    common_options
    utils
)

add_test(NAME testsuite_d_string COMMAND testsuite_d_string)
