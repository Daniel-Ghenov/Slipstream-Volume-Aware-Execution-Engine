add_executable(csvparser_tests
    CSVParserTests.cpp
)

target_link_libraries(csvparser_tests PRIVATE
    csvparser_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(csvparser_tests)
