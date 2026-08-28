add_library(oe_serialisation_lib STATIC
    OESerialiser.cpp
)

target_include_directories(oe_serialisation_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(oe_serialisation_lib PUBLIC
    oe_types
)
