#include "OETypes.h"
#include <cstring>
#include <stdexcept>

using namespace network;

OEMessageType oeMessageTypeFromNum(uint8_t type) {
    switch(type) {
        case 10: return OEMessageType::NEW_ORDER;
        case 11: return OEMessageType::EXEC_REPORT;
        default: throw std::logic_error("Incorrect OE message type recieved");
    }
}

uint8_t oeMessageTypeToNum(OEMessageType type) {
    switch(type) {
        case OEMessageType::NEW_ORDER: return 10;
        case OEMessageType::EXEC_REPORT: return 11;
        default: throw std::logic_error("Incorrect OE message type recieved");
    }
}

OrderStatus orderStatusFromChar(char status) {
    switch(status) {
        case 'A': return OrderStatus::ACCEPTED;
        case 'R': return OrderStatus::REJECTED;
        default: throw std::logic_error("Incorrect order status recieved");
    }
}

char orderStatusToChar(OrderStatus status) {
    switch(status) {
        case OrderStatus::ACCEPTED: return 'A';
        case OrderStatus::REJECTED: return 'R';
        default: throw std::logic_error("Incorrect order status recieved");
    }
}

OrderSide orderSideFromChar(char side) {
    switch(side) {
        case 'B': return OrderSide::BUY;
        case 'S': return OrderSide::SELL;
        default: throw std::logic_error("Incorrect order side recieved");
    }
}

char orderSideToChar(OrderSide side) {
    switch(side) {
        case OrderSide::BUY: return 'B';
        case OrderSide::SELL: return 'S';
        default: throw std::logic_error("Incorrect order side recieved");
    }
}

ExecStatus execStatusFromNum(uint8_t status) {
    switch(status) {
        case 0: return ExecStatus::ACK;
        case 1: return ExecStatus::FILL;
        case 2: return ExecStatus::PARTIAL;
        case 3: return ExecStatus::REJECT;
        default: throw std::logic_error("Incorrect exec status recieved");
    }
}

uint8_t execStatusToNum(ExecStatus status) {
    switch(status) {
        case ExecStatus::ACK: return 0;
        case ExecStatus::FILL: return 1;
        case ExecStatus::PARTIAL: return 2;
        case ExecStatus::REJECT: return 3;
        default: throw std::logic_error("Incorrect exec status recieved");
    }
}

RejectReason rejectReasonFromNum(uint8_t code) {
    switch(code) {
        case 0: return RejectReason::NONE;
        case 1: return RejectReason::RISK;
        case 2: return RejectReason::PRICE;
        case 3: return RejectReason::SIZE;
        case 4: return RejectReason::THROTTLE;
        default: throw std::logic_error("Incorrect reject reason recieved");
    }
}

uint8_t rejectReasonToNum(RejectReason code) {
    switch(code) {
        case RejectReason::NONE: return 0;
        case RejectReason::RISK: return 1;
        case RejectReason::PRICE: return 2;
        case RejectReason::SIZE: return 3;
        case RejectReason::THROTTLE: return 4;
        default: throw std::logic_error("Incorrect reject reason recieved");
    }
}

NewOrder::NewOrder(uint64_t clientOrderId, const char (&symbol)[12], OrderStatus status, uint64_t tsNs, int64_t tradeId, OrderSide side, uint32_t qty, int64_t limitPx)
    : clientOrderId(clientOrderId), status(status), tsNs(tsNs), tradeId(tradeId), side(side), qty(qty), limitPx(limitPx) {
    std::memcpy(this->symbol, symbol, sizeof(this->symbol));
}

NewOrder toNewOrder(const NewOrderBody& body) {
    return NewOrder(body.clientOrderId, body.symbol, orderStatusFromChar(body.status), body.tsNs, body.tradeId, orderSideFromChar(body.side), body.qty, body.limitPx);
}

network::NewOrderBody NewOrder::toBody() const {
    
    network::NewOrderBody body{};
    body.clientOrderId = clientOrderId;
    std::memcpy(body.symbol, symbol, sizeof(symbol));
    body.status = orderStatusToChar(status);
    body.tsNs = tsNs;
    body.tradeId = tradeId;
    body.side = orderSideToChar(side);
    body.qty = qty;
    body.limitPx = limitPx;
    return body;
}

ExecReport toExecReport(const ExecReportBody& body) {
    ExecReport report{};
    report.clientOrderId = body.clientOrderId;
    report.tsNs = body.tsNs;
    report.status = execStatusFromNum(body.status);
    report.filledQty = body.filledQty;
    report.avgPx = body.avgPx;
    report.reasonCode = rejectReasonFromNum(body.reasonCode);
    return report;
}

network::ExecReportBody ExecReport::toBody() const {
    network::ExecReportBody body{};
    body.clientOrderId = clientOrderId;
    body.tsNs = tsNs;
    body.status = execStatusToNum(status);
    body.filledQty = filledQty;
    body.avgPx = avgPx;
    body.reasonCode = rejectReasonToNum(reasonCode);
    return body;
}
