add_library(networking_lib STATIC
    ClientConnection.cpp
    ServerConnection.cpp
)

target_include_directories(networking_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(framing)
add_subdirectory(libslipstream_codec)
add_subdirectory(gcmd)
add_subdirectory(oe)
