cmake_minimum_required(VERSION 3.8)
project(42sh VERSION 1.0.0 LANGUAGES C)

add_library(common_options INTERFACE)

target_compile_options(common_options INTERFACE
    "-Wall"
    "-Wextra"
    "-pedantic"
    $<IF:$<CONFIG:Debug>,-fsanitize=address,-Werror>
)

target_link_libraries(common_options INTERFACE
    $<IF:$<CONFIG:Debug>,-fsanitize=address,-Werror>
)
target_compile_definitions(common_options INTERFACE
    "_GNU_SOURCE"
)

add_subdirectory(src)
add_subdirectory(src/arithmetic)
add_subdirectory(src/ast)
add_subdirectory(src/builtins)
add_subdirectory(src/eval)
add_subdirectory(src/exec)
add_subdirectory(src/expansion)
add_subdirectory(src/lexer)
add_subdirectory(src/options)
add_subdirectory(src/parser)
add_subdirectory(src/shell)
add_subdirectory(src/utils)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Check for criterion
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
    find_package(Criterion REQUIRED)

    # Add tests
    enable_testing()

    # Unit tests
    add_subdirectory(tests/unittests/lexer)
    add_subdirectory(tests/unittests/parser)
    add_subdirectory(tests/unittests/list)
    add_subdirectory(tests/unittests/d_string)
endif ()
