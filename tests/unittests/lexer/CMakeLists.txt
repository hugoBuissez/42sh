add_executable(testsuite_lexer tests_lexer.c)

set_target_properties(testsuite_lexer
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

target_link_libraries(testsuite_lexer PRIVATE
    ${CRITERION_LIBRARIES}
    common_options
    shell
    options
    lexer
    utils
)

add_test(NAME testsuite_lexer COMMAND testsuite_lexer)
