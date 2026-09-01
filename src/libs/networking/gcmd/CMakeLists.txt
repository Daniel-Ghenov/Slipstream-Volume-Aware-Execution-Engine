add_library(gcmd_lib STATIC
    types/GCMDTypes.cpp
    serialisation/GCMDSerialiser.cpp
    deserialisation/GCMDDeserialiser.cpp
)

target_include_directories(gcmd_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/types
        ${CMAKE_CURRENT_SOURCE_DIR}/serialisation
        ${CMAKE_CURRENT_SOURCE_DIR}/deserialisation
)

target_link_libraries(gcmd_lib PUBLIC
    framing_lib
)
