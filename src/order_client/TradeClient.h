#ifndef TRADE_CLIENT_H
#define TRADE_CLIENT_H

#include "CSVParser.h"
#include "ClientConnection.h"
#include "GCMDTypes.h"

class TradeClient {
private:
    ClientConnection* connection;
    CSVParser* parser;
    void* buffer;
    size_t lastSentHeartbeatMS = 0;
public:

    TradeClient(ClientConnection* connection, CSVParser* parser): connection(connection), parser(parser) {}

    void run();

private:
    void sendTrade(const Trade& trade);
    void sendHeartBeat();
    static size_t getNow();

    static TradeBody bodyFromTrade(const Trade& trade);
    static const size_t HEARTBEAT_PER_MS = 50;
};


#endif // TRADE_CLIENT_H
