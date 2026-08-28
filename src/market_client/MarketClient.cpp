#include "MarketClient.h"
#include "GCMDSerialiser.h"
#include "GCMDTypes.h"
#include <thread>
#include <iostream>


void MarketClient::run() {

    Quote firstQuote = parser->readQuote();
    size_t firstTimestamp = firstQuote.timestamp;
    size_t startTimeMillis = getNow();
    size_t startTimestamp = startTimeMillis - firstTimestamp;

    sendQuote(firstQuote);
    while(!parser->reachedEnd()) {
        try {
            Quote quote = parser->readQuote();
            size_t timeToSend = startTimestamp + quote.timestamp;
            for(size_t now{getNow()}; now < timeToSend; now = getNow()) {
                if (now - lastSentHeartbeatMS > HEARTBEAT_PER_MS)
                    sendHeartBeat();
                std::this_thread::sleep_for(std::chrono::milliseconds(std::min(HEARTBEAT_PER_MS, timeToSend - now)));
            }
            sendQuote(quote);
        } catch (std::out_of_range& err)
        {
            std::cerr << "Market client crashed with error: " << err.what() << std::endl;
            break;
        }
    }
}

void MarketClient::sendQuote(const Quote& quote) {
    QuoteBody body = bodyFromQuote(quote);
    size_t messageSize = GCMDSerialiser::serialiseQuoteMessage(buffer, body);
    connection->sendBuffer(buffer, messageSize);
}

void MarketClient::sendHeartBeat() {
    HeartbeatBody body{getNow()};
    size_t messageSize = GCMDSerialiser::serialiseHeartBeatMessage(buffer, body);
    connection->sendBuffer(buffer, messageSize);
}

size_t MarketClient::getNow() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

QuoteBody MarketClient::bodyFromQuote(const Quote& quote){
    QuoteBody body;
    memcpy(body.symbol, quote.symbol, SYMBOL_MAX_LEN);
    body.timestampNs = getNow();
    body.askQuantity = quote.askQuantity;
    body.askPrice = quote.askPrice;
    body.bidPrice = quote.bidPrice;
    body.bidQuantity = quote.bidQuantity;
    return body;
}
