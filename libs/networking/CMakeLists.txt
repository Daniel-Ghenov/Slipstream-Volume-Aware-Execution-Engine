add_library(networking_lib INTERFACE)

target_include_directories(networking_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
