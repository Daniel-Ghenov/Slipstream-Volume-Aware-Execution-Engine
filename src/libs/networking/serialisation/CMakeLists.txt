add_library(serialisation_lib STATIC
    GCMDSerialiser.cpp
)

target_include_directories(serialisation_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(serialisation_lib PUBLIC
    network_types
)
