#ifndef MARKET_CLIENT_H
#define MARKET_CLIENT_H

#include "CSVParser.h"
#include "ClientConnection.h"
#include "GCMDTypes.h"

class MarketClient {
private:
    ClientConnection* connection;
    CSVParser* parser;
    void* buffer;
    size_t lastSentHeartbeatMS = 0;
public:

    MarketClient(ClientConnection* connection, CSVParser* parser): connection(connection), parser(parser) {}

    void run();

private:
    void sendQuote(const csv::Quote& quote);
    void sendHeartBeat();
    static size_t getNow();
    static size_t getWallClockNow();

    static Quote bodyFromQuote(const csv::Quote& quote);
    static constexpr size_t HEARTBEAT_PER_MS = 50;
};


#endif //MARKET_CLIENT_H
