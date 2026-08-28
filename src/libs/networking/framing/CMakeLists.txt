add_library(framing_lib STATIC
    MessageHandler.cpp
)

target_include_directories(framing_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
