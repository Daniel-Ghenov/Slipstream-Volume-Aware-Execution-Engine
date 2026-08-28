#ifndef GCMD_TYPES_H
#define GCMD_TYPES_H


#include <cstdint>
#include "FrameHeader.h"

#pragma pack(push, 1)

enum class MessageType {
    QUOTE = 1,
    TRADE = 2,
    HEARTBEAT = 3,
    SESSION_CONTROL = 4
};

struct QuoteBody {
    char symbol[12];
    uint64_t timestampNs;
    uint32_t bidQuantity;
    int64_t bidPrice;
    uint32_t askQuantity;
    int64_t askPrice;
};

enum class TradeAggressor {
    BUY,
    SELL,
    UNKNOWN
};

struct TradeBody {
    char symbol[12];
    uint64_t timestampNs;
    uint32_t quantity;
    int64_t price;
    char aggressor;
    int64_t id;
};

struct HeartbeatBody {
    uint64_t timestamp;
};

enum class SessionControlState {
    OPEN = 0,
    HALT = 1,
    CLOSE = 2
};

struct SessionControlBody {
    uint64_t timestamp;
    uint8_t state;
};

#pragma pack(pop)


MessageType messageTypeFromNum(uint8_t type);
uint8_t messageTypeToNum(MessageType type);

TradeAggressor aggressorFromChar(char aggressor);
char aggressorToChar(TradeAggressor aggressor);

SessionControlState sessionControlStateFromNum(uint8_t state);
uint8_t sessionControlStateToNum(SessionControlState aggressor);


// ---- Non-packed domain objects for business logic. Translate a wire body
// into one of these immediately after receiving it, and use these (never
// the packed *Body structs above) past that point: naturally aligned, and
// with wire codes already decoded into their enums.
namespace network {

struct Quote {
    char symbol[12];
    uint64_t timestampNs;
    uint32_t bidQuantity;
    int64_t bidPrice;
    uint32_t askQuantity;
    int64_t askPrice;
};

struct Trade {
    char symbol[12];
    uint64_t timestampNs;
    uint32_t quantity;
    int64_t price;
    TradeAggressor aggressor;
    int64_t id;
};

struct Heartbeat {
    uint64_t timestamp;
};

struct SessionControl {
    uint64_t timestamp;
    SessionControlState state;
};

} // namespace network

network::Quote toQuote(const QuoteBody& body);
network::Trade toTrade(const TradeBody& body);
network::Heartbeat toHeartbeat(const HeartbeatBody& body);
network::SessionControl toSessionControl(const SessionControlBody& body);

#endif
