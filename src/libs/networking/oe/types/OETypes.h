#ifndef OE_TYPES_H
#define OE_TYPES_H


#include <cstdint>
#include "FrameHeader.h"

#pragma pack(push, 1)

enum class OEMessageType {
    NEW_ORDER = 10,
    EXEC_REPORT = 11
};

// NewOrder (slipstream -> OE client), msg_type = 10
struct NewOrderBody {
    uint64_t clientOrderId;
    char symbol[12];
    char status;       // 'A' ACCEPTED / 'R' REJECTED
    uint64_t tsNs;
    int64_t tradeId;
    char side;          // 'B' / 'S'
    uint32_t qty;
    int64_t limitPx;    // fixed point x10,000
};

// ExecReport (slipstream -> OE client), msg_type = 11
struct ExecReportBody {
    uint64_t clientOrderId;
    uint64_t tsNs;
    uint8_t status;      // 0=ACK 1=FILL 2=PARTIAL 3=REJECT
    uint32_t filledQty;
    int64_t avgPx;       // fixed point x10,000
    uint8_t reasonCode;  // 0=none 1=risk 2=price 3=size 4=throttle
};

#pragma pack(pop)


enum class OrderStatus {
    ACCEPTED,
    REJECTED
};

enum class OrderSide {
    BUY,
    SELL
};

enum class ExecStatus {
    ACK,
    FILL,
    PARTIAL,
    REJECT
};

enum class RejectReason {
    NONE,
    RISK,
    PRICE,
    SIZE,
    THROTTLE
};

OEMessageType oeMessageTypeFromNum(uint8_t type);
uint8_t oeMessageTypeToNum(OEMessageType type);

OrderStatus orderStatusFromChar(char status);
char orderStatusToChar(OrderStatus status);

OrderSide orderSideFromChar(char side);
char orderSideToChar(OrderSide side);

ExecStatus execStatusFromNum(uint8_t status);
uint8_t execStatusToNum(ExecStatus status);

RejectReason rejectReasonFromNum(uint8_t code);
uint8_t rejectReasonToNum(RejectReason code);


// ---- Non-packed domain objects for business logic. Translate a wire body
// into one of these immediately after receiving it, and use these (never
// the packed *Body structs above) past that point: naturally aligned, and
// with wire codes already decoded into their enums.
namespace network {

struct NewOrder {
    uint64_t clientOrderId;
    char symbol[12];
    OrderStatus status;
    uint64_t tsNs;
    int64_t tradeId;
    OrderSide side;
    uint32_t qty;
    int64_t limitPx;
};

struct ExecReport {
    uint64_t clientOrderId;
    uint64_t tsNs;
    ExecStatus status;
    uint32_t filledQty;
    int64_t avgPx;
    RejectReason reasonCode;
};

} // namespace network

network::NewOrder toNewOrder(const NewOrderBody& body);
network::ExecReport toExecReport(const ExecReportBody& body);

#endif //OE_TYPES_H
