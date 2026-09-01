
#include "NetworkMessageReceiver.h"
#include "OrderBookService.h"
#include "QuoteMessageHandler.h"
#include "ServerConnection.h"
#include <CLI/CLI.hpp>
#include <string>

int main(int argc, char** argv) {
    CLI::App app{};
    std::string symbol;
    app.add_option("--symbol", symbol, "The symbol to process")
        ->required();

    long long maxQantity;
    app.add_option("--max-quantity", maxQantity, "The maximum quantity for any trade")
        ->required();

    double participationCap;
    app.add_option("--participation-cap", participationCap, "Fraction in [0,1]. Cumulative executed qty must stay ≤ cap × cumulative market volume observed since start.")
        ->required();


    long long vwapWindowMs;
    app.add_option("--vwap-window-ms", vwapWindowMs, "Length of the trailing rolling window used for the VWAP benchmark")
        ->required();


    double bandBps;
    app.add_option("--band-bps", bandBps, "Required edge, in basis points, before you will cross. One basis point is 0.01%.")
        ->required();

    std::string transport;
    app.add_option("--transport", transport, "The protocol to be used for messaging")
        ->required();

    std::string mdHost;
    app.add_option("--md-host", mdHost, "The host for the client responsible for sending quotes")
        ->required();

    int mdPort;
    app.add_option("--md-port", mdPort, "The port for the client responsible for sending quotes")
        ->required();

    std::string oeHost;
    app.add_option("--oe-host", oeHost, "The host for the client responsible for sending orders")
        ->required();

    int oePort;
    app.add_option("--oe-port", oePort, "The port for the client responsible for sending orders")
        ->required();

    CLI11_PARSE(app, argc, argv);

    ServerConnection mdListener(mdHost, mdPort);
    ServerConnection oeListener(oeHost, oePort);

    ClientConnection mdClient = mdListener.accept();
    ClientConnection oeClient = oeListener.accept();

    OrderBookService obService = {symbol};
    QuoteMessageHandler mdHandler = {&obService};
    MessageHandler messageHandler;
    NetworkMessageReceiver mdReceiver = {&mdClient, &messageHandler, &mdHandler};
    mdReceiver.start();

    



    return 0;
}
