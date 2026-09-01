#ifndef GCMD_SERIALISER_H
#define GCMD_SERIALISER_H

#include <algorithm>
#include "FrameHeader.h"
#include "GCMDTypes.h"

class GCMDSerialiser {
private:
    static constexpr size_t QUOTE_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::QuoteBody);
    static constexpr size_t TRADE_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::TradeBody);
    static constexpr size_t HEARTBEAT_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::HeartbeatBody);
    static constexpr size_t SESSION_CONTROL_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::SessionControlBody);
public:

    static size_t serialiseHeartBeatMessage(void* buffer, const Heartbeat& heartbeat);
    static size_t serialiseQuoteMessage(void* buffer, const Quote& quote);
    static size_t serialiseTradeMessage(void* buffer, const Trade& trade);
    static size_t serialiseSessionControlMessage(void* buffer, const SessionControl& sessionControl);

    static constexpr size_t MAX_FRAME_SIZE = std::max({HEARTBEAT_MESSAGE_SIZE, QUOTE_MESSAGE_SIZE, TRADE_MESSAGE_SIZE, SESSION_CONTROL_MESSAGE_SIZE});
private:
    static constexpr size_t PROTOCOL_VERSION = 1;

    static network::QuoteBody toQuoteBody(const Quote& quote);
    static network::TradeBody toTradeBody(const Trade& trade);
    static network::HeartbeatBody toHeartbeatBody(const Heartbeat& heartbeat);
    static network::SessionControlBody toSessionControlBody(const SessionControl& sessionControl);
};


#endif //GCMD_SERIALISER_H
