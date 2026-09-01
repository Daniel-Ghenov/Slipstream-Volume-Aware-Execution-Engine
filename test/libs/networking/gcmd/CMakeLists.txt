add_executable(gcmd_codec_tests
    GCMDCodecTests.cpp
)

target_link_libraries(gcmd_codec_tests PRIVATE
    gcmd_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gcmd_codec_tests)
