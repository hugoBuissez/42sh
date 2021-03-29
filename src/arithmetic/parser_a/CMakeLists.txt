add_library(parser_a STATIC
    parse_and.c
    parse_logic_and.c
    parse_mul.c
    parse_parenthethis.c
    parse_xor.c
    parser_a.h
    parse_add.c
    parse_expo.c
    parse_logic_or.c
    parse_or.c
    parse_negative.c
    parse_positive.c
    parse_bitwise_not.c
    parse_unit.c
    parser_a.c
)

target_include_directories(parser_a PUBLIC .)

target_link_libraries(parser_a PRIVATE
    common_options
    ast_a
    lexer_a
    utils_a
)
