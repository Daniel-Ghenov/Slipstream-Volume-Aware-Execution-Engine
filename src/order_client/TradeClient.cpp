#include "TradeClient.h"
#include "GCMDSerialiser.h"
#include "GCMDTypes.h"
#include <thread>
#include <iostream>


void TradeClient::run() {

    Trade firstTrade = parser->readTrade();
    size_t firstTimestamp = firstTrade.timestamp;
    size_t startTimeMillis = getNow();
    size_t startTimestamp = startTimeMillis - firstTimestamp;

    sendTrade(firstTrade);
    while(!parser->reachedEnd()) {
        try {
            Trade trade = parser->readTrade();
            size_t timeToSend = startTimestamp + trade.timestamp;
            for(size_t now{getNow()}; now < timeToSend; now = getNow()) {
                if (now - lastSentHeartbeatMS > HEARTBEAT_PER_MS)
                    sendHeartBeat();
                std::this_thread::sleep_for(std::chrono::milliseconds(std::min(HEARTBEAT_PER_MS, timeToSend - now)));
            }
            sendTrade(trade);
        } catch (std::out_of_range& err)
        {
            std::cerr << "Market client crashed with error: " << err.what() << std::endl;
            break;
        }
    }
}

void TradeClient::sendTrade(const Trade& trade) {
    TradeBody body = bodyFromTrade(trade);
    size_t messageSize = GCMDSerialiser::serialiseTradeMessage(buffer, body);
    connection->sendBuffer(buffer, messageSize);
}

void TradeClient::sendHeartBeat() {
    HeartbeatBody body{getNow()};
    size_t messageSize = GCMDSerialiser::serialiseHeartBeatMessage(buffer, body);
    connection->sendBuffer(buffer, messageSize);
}

size_t TradeClient::getNow() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

TradeBody TradeClient::bodyFromTrade(const Trade& trade){
    TradeBody body;
    memcpy(body.symbol, trade.symbol, SYMBOL_MAX_LEN);
    body.timestampNs = getNow();
    body.price = trade.price;
    body.quantity = trade.quantity;
    return body;
}
