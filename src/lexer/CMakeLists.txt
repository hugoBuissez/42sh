add_library(lexer STATIC
    token.h
    token.c
    token_classification.c
    lexer.h
    lexer.c
    lexer_rules.h
    lexer_rules_1.c
    lexer_rules_2.c
    lexer_internals.h
    lexer_internals.c
    lexer_consume.c
    lexer_management.c
    lexer_usage.c
    lexer_printer.c
)

target_include_directories(lexer PUBLIC .)

target_link_libraries(lexer PRIVATE
    common_options
    options
    shell
    utils
)
