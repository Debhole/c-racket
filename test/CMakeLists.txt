add_executable(c_racket_test
        test_runner.c
        test_token_list.h test_macros.h test_scanner.h test_token.h test_ast.h test_ast_list.h test_interpreter.h)

target_link_libraries(c_racket_test PRIVATE c_racket_lib)
target_include_directories(c_racket_test
        PRIVATE
        .)

add_test(
        NAME c_racket_test
        COMMAND c_racket_test
)