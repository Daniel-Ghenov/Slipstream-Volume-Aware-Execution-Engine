add_executable(slipstream
    main.cpp
    network/NetworkMessageReceiver.cpp
    market/OrderBookService.cpp
    market/QuoteMessageHandler.cpp
    orders/TradeMessageHandler.cpp
    orders/VWAPService.cpp
)

target_include_directories(slipstream PRIVATE
    network
    market
    orders
    engine
)

target_link_libraries(slipstream PRIVATE
    csvparser_lib
    networking_lib
    libslipstream_codec
    gcmd_lib
    CLI11::CLI11
)
