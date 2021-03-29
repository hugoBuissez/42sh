add_library(utils_a STATIC
    xalloc.h
    xalloc.c
)

target_include_directories(utils_a PUBLIC .)

target_link_libraries(utils_a PRIVATE
    common_options
)
