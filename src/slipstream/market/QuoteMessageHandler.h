#ifndef QUOTE_MESSAGE_HANDLER_H
#define QUOTE_MESSAGE_HANDLER_H


#include "OrderBookService.h"
#include "MessageHandler.h"
#include "ExecutionEngine.h"

class QuoteMessageHandler : public MessageHandler {
private:
    OrderBookService* orderBookService;
    ExecutionEngine* executionEngine;

public:
    QuoteMessageHandler(OrderBookService* orderBookService, ExecutionEngine* executionEngine)
        : orderBookService(orderBookService), executionEngine(executionEngine) {}

    QuoteMessageHandler(const QuoteMessageHandler&) = delete;

    virtual void onMessage(const MDMessage&) override;
    virtual bool canHandleMessage(const MDMessage&) override;
};



#endif // QUOTE_MESSAGE_HANDLER_H
