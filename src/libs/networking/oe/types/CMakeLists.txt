add_library(oe_types STATIC
    OETypes.cpp
)

target_include_directories(oe_types
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(oe_types PUBLIC
    framing_lib
)
