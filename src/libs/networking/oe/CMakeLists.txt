add_library(oe_lib STATIC
    types/OETypes.cpp
    serialisation/OESerialiser.cpp
    deserialisation/OEDeserialiser.cpp
)

target_include_directories(oe_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/types
        ${CMAKE_CURRENT_SOURCE_DIR}/serialisation
        ${CMAKE_CURRENT_SOURCE_DIR}/deserialisation
)

target_link_libraries(oe_lib PUBLIC
    framing_lib
)
