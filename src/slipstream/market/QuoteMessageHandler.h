#ifndef QUOTE_MESSAGE_HANDLER_H
#define QUOTE_MESSAGE_HANDLER_H


#include "OrderBookService.h"
#include "NetworkMessageHandler.h"

class QuoteMessageHandler : public NetworkMessageHandler {
private:
    OrderBookService* orderBookService;

public:
    QuoteMessageHandler(OrderBookService* orderBookService);

    QuoteMessageHandler(const QuoteMessageHandler&) = delete;

    virtual void onMessage(const MDMessage&) override;
};



#endif // QUOTE_MESSAGE_HANDLER_H
