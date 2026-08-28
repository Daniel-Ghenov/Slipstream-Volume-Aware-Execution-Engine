add_executable(market_client
    main.cpp
)

target_link_libraries(market_client PRIVATE
    csvparser_lib
    networking_lib
    serialisation_lib
)
