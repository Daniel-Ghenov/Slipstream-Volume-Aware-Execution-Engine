add_library(libslipstream_codec STATIC
    GCMDDeserialiser.cpp
)

target_include_directories(libslipstream_codec
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libslipstream_codec PUBLIC
    network_types
)
