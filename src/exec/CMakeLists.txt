add_library(exec STATIC
    exec.h
    exec_simple_command.c
    exec_pipe.c
)

target_include_directories(exec PUBLIC .)

target_link_libraries(exec PRIVATE
    common_options
    ast
    builtins
    eval
    lexer
    options
    parser
    shell
    utils
)
