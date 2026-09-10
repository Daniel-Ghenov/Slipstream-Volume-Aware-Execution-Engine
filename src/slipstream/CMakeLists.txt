add_executable(slipstream
    main.cpp
    SessionControlHandler.cpp
    network/NetworkMessageReceiver.cpp
    network/OrderMessageSender.cpp
    network/MasterMessageHandler.cpp
    network/MessageDeMultiplexer.cpp
    market/OrderBookService.cpp
    market/QuoteMessageHandler.cpp
    orders/TradeMessageHandler.cpp
    orders/VWAPService.cpp
    engine/ExecutionEngine.cpp
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
    concurrency_lib
    CLI11::CLI11
    oe_lib
)
