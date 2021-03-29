add_library(eval STATIC
    eval.h
    eval.c
    eval_operator.c
    eval_input.c
    eval_pipeline.c
    eval_command.c
    eval_simple_command.c
    eval_for.c
    eval_while.c
    eval_until.c
    eval_case.c
    eval_if.c
    redirections.c
    redirections.h
)

target_include_directories(eval PUBLIC .)

target_link_libraries(eval PRIVATE
    common_options
    ast
    builtins
    exec
    expansion
    parser
    lexer
    options
    shell
    utils
)
