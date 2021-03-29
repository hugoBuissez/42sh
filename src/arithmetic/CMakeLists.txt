add_library(arithmetic STATIC
    arithmetic.c
    arithmetic.h
)

target_include_directories(arithmetic PUBLIC .)

target_link_libraries(arithmetic PRIVATE
    common_options
    ast_a
    eval_a
    lexer_a
    parser_a
    utils_a
)

add_subdirectory(ast_a)
add_subdirectory(eval_a)
add_subdirectory(lexer_a)
add_subdirectory(parser_a)
add_subdirectory(utils_a)
