add_executable(oe_codec_tests
    OECodecTests.cpp
)

target_link_libraries(oe_codec_tests PRIVATE
    oe_types
    oe_serialisation_lib
    oe_deserialisation_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(oe_codec_tests)
