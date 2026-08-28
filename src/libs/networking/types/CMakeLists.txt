add_library(network_types STATIC
    GCMDTypes.cpp
)

target_include_directories(network_types
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(network_types PUBLIC
    framing_lib
)
