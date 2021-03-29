add_library(builtins STATIC
    builtins.h
    builtins.c
    echo.c
    cd.c
    exit.c
    export.c
    source.c
    alias.c
)

target_include_directories(builtins PUBLIC .)

target_link_libraries(builtins PRIVATE
    common_options
    ast
    eval
    lexer
    options
    parser
    shell
    utils
)
