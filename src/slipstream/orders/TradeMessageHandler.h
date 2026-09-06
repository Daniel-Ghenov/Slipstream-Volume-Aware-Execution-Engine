#ifndef TRADE_MESSAGE_HANDLER_H
#define TRADE_MESSAGE_HANDLER_H

#include "ExecutionEngine.h"
#include "MessageHandler.h"
#include "VWAPService.hpp"
#include "OrderMessageSender.h"

class TradeMessageHandler : public MessageHandler {
private:
    VWAPService* vwapService;
    ExecutionEngine* executionEngine;
    OrderMessageSender* orderMessageSender;

public:
    TradeMessageHandler(VWAPService* vwapService, ExecutionEngine* executionEngine)
        : vwapService(vwapService), executionEngine(executionEngine) {}

    TradeMessageHandler(const TradeMessageHandler&) = delete;

    virtual void onMessage(const MDMessage&) override;
    virtual bool canHandleMessage(const MDMessage&) override;
};

#endif // TRADE_MESSAGE_HANDLER_H
