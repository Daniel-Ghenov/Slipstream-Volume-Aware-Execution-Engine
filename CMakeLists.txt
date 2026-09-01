cmake_minimum_required(VERSION 3.31)
project(SlipstreamVolumeAwareExecutionEngine)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

option(SLIPSTREAM_BUILD_TESTS "Build the test suite" OFF)

include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.7.2
)
FetchContent_MakeAvailable(cli11)

if(SLIPSTREAM_BUILD_TESTS)
    enable_testing()

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)
endif()


add_subdirectory(src)

if(SLIPSTREAM_BUILD_TESTS)
    add_subdirectory(test)
endif()
