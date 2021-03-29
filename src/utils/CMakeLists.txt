add_library(utils STATIC
    list.h
    list_internals.h
    list_management.c
    list_usage.c
    list_iterator.c
    argv_list.h
    argv_list.c
    argv_list_aux.c
    htab.c
    htab_basics.c
    htab_iterator.c
    string_utils.h
    string_utils.c
    string_utils_aux.c
    dynamic_string.h
    dynamic_string_internals.h
    dynamic_string.c
    dynamic_string_aux.c
    dynamic_string_utils.c
)

target_include_directories(utils PUBLIC .)

target_link_libraries(utils PRIVATE
    common_options
)
