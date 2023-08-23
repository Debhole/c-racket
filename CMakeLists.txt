cmake_minimum_required(VERSION 3.25)
project(c_racket C)

set(CMAKE_C_STANDARD 11)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(c_racket_lib
        src/tokens/token_type.h
        src/scanner/scanner.c
        src/scanner/scanner.h
        src/tokens/token_list.c
        src/tokens/token_list.h
        src/tokens/token.c
        src/tokens/token.h
        src/number.c
        src/number.h
        src/ast/ast.c
        src/ast/ast.h
        src/ast/ast_builder.c
        src/ast/ast_builder.h
        src/keyword.c
        src/keyword.h
        src/tokens/token_type.c src/ast/ast_list.c src/ast/ast_list.h)
target_include_directories(c_racket_lib PUBLIC src)

add_executable(c_racket src/main.c)

target_link_libraries(c_racket PRIVATE c_racket_lib)
target_include_directories(c_racket PRIVATE src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(test)
    endif ()
endif ()