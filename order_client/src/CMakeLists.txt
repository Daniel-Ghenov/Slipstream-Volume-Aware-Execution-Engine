add_executable(order_client
    main.cpp
)

target_link_libraries(order_client PRIVATE
    csvparser_lib
    networking_lib
    serialisation_lib
)
