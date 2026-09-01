#include "GCMDTypes.h"
#include <cstring>
#include <stdexcept>

using namespace network;

MessageType messageTypeFromNum(uint8_t type) {
    switch(type) {
        case 1: return MessageType::QUOTE;
        case 2: return MessageType::TRADE;
        case 3: return MessageType::HEARTBEAT;
        case 4: return MessageType::SESSION_CONTROL;
        default: throw std::logic_error("Incorrect message type recieved");
    }
}

uint8_t messageTypeToNum(MessageType type) {
    switch(type) {
        case MessageType::QUOTE: return 1;
        case MessageType::TRADE: return 2;
        case MessageType::HEARTBEAT: return 3;
        case MessageType::SESSION_CONTROL: return 4;
        default: throw std::logic_error("Incorrect message type recieved");
    }
}

TradeAggressor aggressorFromChar(char aggressor) {
    switch(aggressor) {
        case 'B': return TradeAggressor::BUY;
        case 'S': return TradeAggressor::SELL;
        case '?': return TradeAggressor::UNKNOWN;
        default: throw std::logic_error("Incorrect trade agressor recieved");
    }
}

char aggressorToChar(TradeAggressor aggressor) {
    switch(aggressor) {
        case TradeAggressor::BUY: return 'B';
        case TradeAggressor::SELL: return 'S';
        case TradeAggressor::UNKNOWN: return '?';
        default: throw std::logic_error("Incorrect trade agressor recieved");
    }
}

SessionControlState sessionControlStateFromNum(uint8_t state) {
    switch(state) {
        case 0: return SessionControlState::OPEN;
        case 1: return SessionControlState::HALT;
        case 2: return SessionControlState::CLOSE;
        default: throw std::logic_error("Incorrect session control recieved");
    }
}

uint8_t sessionControlStateToNum(SessionControlState aggressor) {
    switch(aggressor) {
        case SessionControlState::OPEN: return 0;
        case SessionControlState::CLOSE: return 1;
        case SessionControlState::HALT: return 2;
        default: throw std::logic_error("Incorrect session control recieved");
    }
}

Quote toQuote(const QuoteBody& body) {
    Quote quote{};
    std::memcpy(quote.symbol, body.symbol, sizeof(quote.symbol));
    quote.timestampNs = body.timestampNs;
    quote.bidQuantity = body.bidQuantity;
    quote.bidPrice = body.bidPrice;
    quote.askQuantity = body.askQuantity;
    quote.askPrice = body.askPrice;
    return quote;
}

Trade toTrade(const TradeBody& body) {
    Trade trade{};
    std::memcpy(trade.symbol, body.symbol, sizeof(trade.symbol));
    trade.timestampNs = body.timestampNs;
    trade.quantity = body.quantity;
    trade.price = body.price;
    trade.aggressor = aggressorFromChar(body.aggressor);
    trade.id = body.id;
    return trade;
}

Heartbeat toHeartbeat(const HeartbeatBody& body) {
    return Heartbeat{body.timestamp};
}

SessionControl toSessionControl(const SessionControlBody& body) {
    return SessionControl{body.timestamp, sessionControlStateFromNum(body.state)};
}
