add_library(ast_a STATIC
    ast_a.c
    ast_a.h
)

target_include_directories(ast_a PUBLIC .)

target_link_libraries(ast_a PRIVATE
    common_options
    utils_a
)
