#ifndef GCMD_TYPES_H
#define GCMD_TYPES_H


#include <cstdint>
#include <variant>

namespace network {

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

struct SessionControlBody {
    uint64_t timestamp;
    uint8_t state;
};

#pragma pack(pop)

} // namespace network


enum class TradeAggressor {
    BUY,
    SELL,
    UNKNOWN
};

enum class SessionControlState {
    OPEN = 0,
    HALT = 1,
    CLOSE = 2
};


network::MessageType messageTypeFromNum(uint8_t type);
uint8_t messageTypeToNum(network::MessageType type);

TradeAggressor aggressorFromChar(char aggressor);
char aggressorToChar(TradeAggressor aggressor);

SessionControlState sessionControlStateFromNum(uint8_t state);
uint8_t sessionControlStateToNum(SessionControlState aggressor);


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


using MDMessage = std::variant<Quote, Trade, Heartbeat, SessionControl>;

Quote toQuote(const network::QuoteBody& body);
Trade toTrade(const network::TradeBody& body);
Heartbeat toHeartbeat(const network::HeartbeatBody& body);
SessionControl toSessionControl(const network::SessionControlBody& body);

#endif
