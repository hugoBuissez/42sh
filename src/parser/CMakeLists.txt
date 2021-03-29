add_library(parser STATIC
    parser.h
    parser.c
    parser_management.c
    rule_input.c
    rule_list.c
    rule_and_or.c
    rule_pipeline.c
    rule_command.c
    rule_simple_command.c
    rule_shell_command.c
    rule_prefix.c
    rule_element.c
    rule_redirection.c
    rule_compound_list.c
    rule_if.c
    rule_while.c
    rule_until.c
    rule_do_group.c
    rule_for.c
    rule_funcdec.c
    rule_case.c
    rule_case_clause.c
    rule_case_item.c
    rule_else_clause.c
)

target_include_directories(parser PUBLIC .)

target_link_libraries(parser PRIVATE
    common_options
    ast
    lexer
    options
    shell
    utils
)
