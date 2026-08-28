add_library(csvparser_lib STATIC
    CSVParser.cpp
)

target_include_directories(csvparser_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
