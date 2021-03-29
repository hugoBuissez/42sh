add_executable(testsuite_parser tests_parser.c)

set_target_properties(testsuite_parser
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

target_link_libraries(testsuite_parser PRIVATE
    ${CRITERION_LIBRARIES}
    common_options
    ast
    options
    parser
    lexer
    shell
    utils
)

add_test(NAME testsuite_parser COMMAND testsuite_parser)
