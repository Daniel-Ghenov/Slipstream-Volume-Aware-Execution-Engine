#include "GCMDDeserialiser.h"
#include <stdexcept>

using namespace network;

Heartbeat GCMDDeserialiser::getHeartBeatMessage(void* buffer) {
    HeartbeatBody* body = reinterpret_cast<HeartbeatBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return toHeartbeat(*body);
}
Quote GCMDDeserialiser::getQuoteMessage(void* buffer) {
    QuoteBody* body = reinterpret_cast<QuoteBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return toQuote(*body);
}
Trade GCMDDeserialiser::getTradeMessage(void* buffer) {
    TradeBody* body = reinterpret_cast<TradeBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return toTrade(*body);
}
SessionControl GCMDDeserialiser::getSessionControlMessage(void* buffer) {
    SessionControlBody* body = reinterpret_cast<SessionControlBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return toSessionControl(*body);
}

MDMessage GCMDDeserialiser::deserialiseMessage(void* buffer) {
    switch (getMessageType(buffer)) {
        case MessageType::QUOTE:
            return getQuoteMessage(buffer);
        case MessageType::TRADE:
            return getTradeMessage(buffer);
        case MessageType::HEARTBEAT:
            return getHeartBeatMessage(buffer);
        case MessageType::SESSION_CONTROL:
            return getSessionControlMessage(buffer);
    }
    throw std::logic_error("Unknown message type");
}

size_t GCMDDeserialiser::getBodySize(void* buffer) {
    FrameHeader* header = static_cast<FrameHeader*>(buffer);
    return header->bodyLen;
}

size_t GCMDDeserialiser::getMessageSize(void* buffer) {
    return sizeof(FrameHeader) + getBodySize(buffer);
}
MessageType GCMDDeserialiser::getMessageType(void* buffer) {
    FrameHeader* header = static_cast<FrameHeader*>(buffer);
    return messageTypeFromNum(header->msgType);
}

bool GCMDDeserialiser::isCompleteHeader(size_t size) {
    return size >= sizeof(FrameHeader);
}
