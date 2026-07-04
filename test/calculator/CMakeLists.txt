# Calculator parser unit tests (ported from CFDesktop's test/desktop/calculator).
# 53 cases covering basic arithmetic, precedence, unary negation, functions,
# nesting, edge cases, and error throws.
add_executable(parser_test parser_test.cpp)

target_link_libraries(parser_test PRIVATE
    calculator_parser
    Qt6::Core
    GTest::gtest
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(parser_test)
