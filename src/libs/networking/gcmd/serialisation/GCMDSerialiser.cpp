#include "GCMDSerialiser.h"
#include "GCMDTypes.h"
#include <cstring>

using namespace network;

size_t GCMDSerialiser::serialiseHeartBeatMessage(void* buffer, const Heartbeat& heartbeat) {
    HeartbeatBody body = toHeartbeatBody(heartbeat);
    FrameHeader header = {sizeof(HeartbeatBody), messageTypeToNum(MessageType::HEARTBEAT), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(HeartbeatBody));
    return HEARTBEAT_MESSAGE_SIZE;
}
size_t GCMDSerialiser::serialiseQuoteMessage(void* buffer, const Quote& quote) {
    QuoteBody body = toQuoteBody(quote);
    FrameHeader header = {sizeof(QuoteBody), messageTypeToNum(MessageType::QUOTE), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(QuoteBody));
    return QUOTE_MESSAGE_SIZE;
}
size_t GCMDSerialiser::serialiseTradeMessage(void* buffer, const Trade& trade) {
    TradeBody body = toTradeBody(trade);
    FrameHeader header = {sizeof(TradeBody), messageTypeToNum(MessageType::TRADE), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(TradeBody));
    return TRADE_MESSAGE_SIZE;
}
size_t GCMDSerialiser::serialiseSessionControlMessage(void* buffer, const SessionControl& sessionControl) {
    SessionControlBody body = toSessionControlBody(sessionControl);
    FrameHeader header = {sizeof(SessionControlBody), messageTypeToNum(MessageType::SESSION_CONTROL), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(SessionControlBody));
    return SESSION_CONTROL_MESSAGE_SIZE;
}

QuoteBody GCMDSerialiser::toQuoteBody(const Quote& quote) {
    QuoteBody body{};
    std::memcpy(body.symbol, quote.symbol, sizeof(body.symbol));
    body.timestampNs = quote.timestampNs;
    body.bidQuantity = quote.bidQuantity;
    body.bidPrice = quote.bidPrice;
    body.askQuantity = quote.askQuantity;
    body.askPrice = quote.askPrice;
    return body;
}

TradeBody GCMDSerialiser::toTradeBody(const Trade& trade) {
    TradeBody body{};
    std::memcpy(body.symbol, trade.symbol, sizeof(body.symbol));
    body.timestampNs = trade.timestampNs;
    body.quantity = trade.quantity;
    body.price = trade.price;
    body.aggressor = aggressorToChar(trade.aggressor);
    body.id = trade.id;
    return body;
}

HeartbeatBody GCMDSerialiser::toHeartbeatBody(const Heartbeat& heartbeat) {
    return HeartbeatBody{heartbeat.timestamp};
}

SessionControlBody GCMDSerialiser::toSessionControlBody(const SessionControl& sessionControl) {
    return SessionControlBody{sessionControl.timestamp, sessionControlStateToNum(sessionControl.state)};
}
