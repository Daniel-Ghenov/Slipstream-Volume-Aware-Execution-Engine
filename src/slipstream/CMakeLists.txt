add_executable(slipstream
    main.cpp
)

target_link_libraries(slipstream PRIVATE
    csvparser_lib
    networking_lib
    serialisation_lib
    CLI11::CLI11
)
