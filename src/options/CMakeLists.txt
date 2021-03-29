add_library(options STATIC
    options.h
    options.c
)

target_include_directories(options PUBLIC .)

target_link_libraries(options PRIVATE
    common_options
    utils
)
