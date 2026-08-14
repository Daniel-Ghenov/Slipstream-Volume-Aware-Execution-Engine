#ifndef GCMD_TYPES_H
#define GCMD_TYPES_H


#include <cstdint>

#pragma pack(push, 1)

enum class MessageType {
    QUOTE = 1,
    TRADE = 2,
    HEARTBEAT = 3,
    SESSION_CONTROL = 4
};

struct FrameHeader {
    uint16_t bodyLen;
    uint8_t msgType;
    uint8_t version;
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

#endif
