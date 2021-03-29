add_library(expansion STATIC
    expansion.h
    expansion.c
    expansion_aux.c
    expand_parameter.c
    command_sub.c
    quote_removal.c
    word_splitting.c
    expand_pathname.c
)

target_include_directories(expansion PUBLIC .)

target_link_libraries(expansion PRIVATE
    common_options
    arithmetic
    lexer
    options
    shell
    utils
)
