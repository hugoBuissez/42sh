add_library(lexer_a STATIC
    lexer_a.c
    lexer_a.h
    lexer_struct.h
    token_a.h
    token_a.c
)

target_include_directories(lexer_a PUBLIC .)

target_link_libraries(lexer_a PRIVATE
    common_options
    utils_a
)
