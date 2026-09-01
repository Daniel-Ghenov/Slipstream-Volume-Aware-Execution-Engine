add_executable(message_handler_tests
    MessageHandlerTests.cpp
)

target_link_libraries(message_handler_tests PRIVATE
    framing_lib
    libslipstream_codec
    gcmd_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(message_handler_tests)
