add_library(eval_a STATIC
    ast_eval_a.c
    ast_eval_a.h
    ast_print_a.c
    token_printer.c
    token_printer.h
)

target_include_directories(eval_a PUBLIC .)

target_link_libraries(eval_a PRIVATE
    common_options
    ast_a
    lexer_a
    parser_a
    utils_a
)
