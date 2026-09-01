add_executable(order_client
    main.cpp
    TradeClient.cpp
)

target_link_libraries(order_client PRIVATE
    csvparser_lib
    networking_lib
    gcmd_lib
)
