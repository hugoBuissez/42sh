add_library(ast STATIC
    ast.h
    ast_printer.h
    ast_printer.c
    dot_printer.c
)

target_include_directories(ast PUBLIC .)

target_link_libraries(ast PRIVATE
    common_options
    lexer
    options
    parser
    shell
    utils
)
