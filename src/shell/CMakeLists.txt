add_library(shell STATIC
    shell.h
    shell.c
    variables.h
    variables.c
    variables_aux.c
    alias.h
    alias.c
)

target_include_directories(shell PUBLIC .)

target_link_libraries(shell PRIVATE
    common_options
    ast
    eval
    exec
    lexer
    options
    parser
    utils
)
