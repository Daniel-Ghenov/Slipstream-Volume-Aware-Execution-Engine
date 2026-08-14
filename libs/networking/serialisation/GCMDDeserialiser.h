
#ifndef GCMD_DESERIALISER_H
#define GCMD_DESERIALISER_H

#include <algorithm>
#include <cstdint>
#include "GCMDTypes.h"

class GCMDDeserialiser {
private:
    static constexpr size_t QUOTE_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(QuoteBody);
    static constexpr size_t TRADE_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(TradeBody);
    static constexpr size_t HEARTBEAT_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(HeartbeatBody);
    static constexpr size_t SESSION_CONTROL_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(SessionControlBody);
public:


    HeartbeatBody getHeartBeatMessage(void* buffer);
    QuoteBody getQuoteMessage(void* buffer);
    TradeBody getTradeMessage(void* buffer);
    SessionControlBody getSessionControlMessage(void* buffer);
    size_t getBodySize(void* buffer);
    MessageType getMessageType(void* buffer);
    bool isCompleteHeader(size_t size);

    static constexpr size_t MAX_FRAME_SIZE = std::max({HEARTBEAT_MESSAGE_SIZE, QUOTE_MESSAGE_SIZE, TRADE_MESSAGE_SIZE, SESSION_CONTROL_MESSAGE_SIZE});
private:
    static constexpr size_t PROTOCOL_VERSION = 1;
    uint64_t getNow();
};


#endif //GCMD_DESERIALISER_H
