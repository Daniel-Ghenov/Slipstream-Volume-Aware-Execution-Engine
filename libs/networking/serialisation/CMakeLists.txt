add_library(serialisation_lib STATIC
    GCMDSerialiser.cpp
    GCMDDeserialiser.cpp
    GCMDTypes.cpp
)

target_include_directories(serialisation_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
