add_library(oe_deserialisation_lib STATIC
    OEDeserialiser.cpp
)

target_include_directories(oe_deserialisation_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(oe_deserialisation_lib PUBLIC
    oe_types
)
