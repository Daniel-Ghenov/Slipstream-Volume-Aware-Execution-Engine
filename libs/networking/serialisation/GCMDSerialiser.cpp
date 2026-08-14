#include "GCMDSerialiser.h"
#include "GCMDTypes.h"

size_t GCMDSerialiser::serialiseHeartBeatMessage(void* buffer, HeartbeatBody& body) {
    FrameHeader header = {sizeof(HeartbeatBody), messageTypeToNum(MessageType::HEARTBEAT), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(HeartbeatBody));
    return HEARTBEAT_MESSAGE_SIZE;
}
size_t GCMDSerialiser::serialiseQuoteMessage(void* buffer, QuoteBody& body) {
    FrameHeader header = {sizeof(QuoteBody), messageTypeToNum(MessageType::QUOTE), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(QuoteBody));
    return QUOTE_MESSAGE_SIZE;

}
size_t GCMDSerialiser::serialiseTradeMessage(void* buffer, TradeBody& body) {
    FrameHeader header = {sizeof(TradeBody), messageTypeToNum(MessageType::TRADE), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(TradeBody));
    return TRADE_MESSAGE_SIZE;

}
size_t GCMDSerialiser::serialiseSessionControlMessage(void* buffer, SessionControlBody& body) {
    FrameHeader header = {sizeof(SessionControlBody), messageTypeToNum(MessageType::SESSION_CONTROL), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(SessionControlBody));
    return SESSION_CONTROL_MESSAGE_SIZE;
}
