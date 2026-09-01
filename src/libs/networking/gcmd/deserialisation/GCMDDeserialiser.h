#ifndef GCMD_DESERIALISER_H
#define GCMD_DESERIALISER_H

#include <algorithm>
#include <cstdint>
#include "FrameHeader.h"
#include "GCMDTypes.h"

class GCMDDeserialiser {
private:
    static constexpr size_t QUOTE_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::QuoteBody);
    static constexpr size_t TRADE_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::TradeBody);
    static constexpr size_t HEARTBEAT_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::HeartbeatBody);
    static constexpr size_t SESSION_CONTROL_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::SessionControlBody);
public:

    static Heartbeat getHeartBeatMessage(void* buffer);
    static Quote getQuoteMessage(void* buffer);
    static Trade getTradeMessage(void* buffer);
    static SessionControl getSessionControlMessage(void* buffer);

    static MDMessage deserialiseMessage(void* buffer);

    static size_t getBodySize(void* buffer);
    static size_t getMessageSize(void* buffer);
    static bool isCompleteHeader(size_t size);

    static constexpr size_t MAX_FRAME_SIZE = std::max({HEARTBEAT_MESSAGE_SIZE, QUOTE_MESSAGE_SIZE, TRADE_MESSAGE_SIZE, SESSION_CONTROL_MESSAGE_SIZE});
private:
    static constexpr size_t PROTOCOL_VERSION = 1;
    static uint64_t getNow();

    static network::MessageType getMessageType(void* buffer);
};


#endif //GCMD_DESERIALISER_H
