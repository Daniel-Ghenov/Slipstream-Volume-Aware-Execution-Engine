#include "GCMDDeserialiser.h"


HeartbeatBody GCMDDeserialiser::getHeartBeatMessage(void* buffer) {
    HeartbeatBody* body = reinterpret_cast<HeartbeatBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return *body;
}
QuoteBody GCMDDeserialiser::getQuoteMessage(void* buffer) {
    QuoteBody* body = reinterpret_cast<QuoteBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return *body;
}
TradeBody GCMDDeserialiser::getTradeMessage(void* buffer) {
    TradeBody* body = reinterpret_cast<TradeBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return *body;
}
SessionControlBody GCMDDeserialiser::getSessionControlMessage(void* buffer) {
    SessionControlBody* body = reinterpret_cast<SessionControlBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return *body;
}

size_t GCMDDeserialiser::getBodySize(void* buffer) {
    FrameHeader* header = static_cast<FrameHeader*>(buffer);
    return header->bodyLen;
}
MessageType GCMDDeserialiser::getMessageType(void* buffer) {
    FrameHeader* header = static_cast<FrameHeader*>(buffer);
    return messageTypeFromNum(header->msgType);
}

bool GCMDDeserialiser::isCompleteHeader(size_t size) {
    return size >= sizeof(FrameHeader);
}
