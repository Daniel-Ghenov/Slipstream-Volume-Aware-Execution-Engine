add_library(networking_lib STATIC
    ClientConnection.cpp
)

target_include_directories(networking_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(serialisation)
