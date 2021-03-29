add_executable(42sh 42sh.c)

set_target_properties(42sh
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_link_libraries(42sh PRIVATE
    common_options
    shell
    lexer
    options
    utils
)
