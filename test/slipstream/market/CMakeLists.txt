add_executable(order_book_service_tests
    OrderBookServiceTests.cpp
    ../../../src/slipstream/market/OrderBookService.cpp
)

target_include_directories(order_book_service_tests PRIVATE
    ../../../src/slipstream/market
)

target_link_libraries(order_book_service_tests PRIVATE
    gcmd_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(order_book_service_tests)
