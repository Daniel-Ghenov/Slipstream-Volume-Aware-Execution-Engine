#ifndef GCMD_SERIALISER_H
#define GCMD_SERIALISER_H

#include <algorithm>
#include "GCMDTypes.h"

class GCMDSerialiser {
private:
    static constexpr size_t QUOTE_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(QuoteBody);
    static constexpr size_t TRADE_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(TradeBody);
    static constexpr size_t HEARTBEAT_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(HeartbeatBody);
    static constexpr size_t SESSION_CONTROL_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(SessionControlBody);
public:

    static size_t serialiseHeartBeatMessage(void* buffer, HeartbeatBody& body);
    static size_t serialiseQuoteMessage(void* buffer, QuoteBody& body);
    static size_t serialiseTradeMessage(void* buffer, TradeBody& body);
    static size_t serialiseSessionControlMessage(void* buffer, SessionControlBody& body);

    static constexpr size_t MAX_FRAME_SIZE = std::max({HEARTBEAT_MESSAGE_SIZE, QUOTE_MESSAGE_SIZE, TRADE_MESSAGE_SIZE, SESSION_CONTROL_MESSAGE_SIZE});
private:
    static constexpr size_t PROTOCOL_VERSION = 1;
};


#endif //GCMD_SERIALISER_H
