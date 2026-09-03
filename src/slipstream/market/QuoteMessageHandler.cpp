#include "QuoteMessageHandler.h"
#include "GCMDTypes.h"
#include <stdexcept>

QuoteMessageHandler::QuoteMessageHandler(OrderBookService* obService): orderBookService(obService) {}

void QuoteMessageHandler::onMessage(const MDMessage& message) {

    const Quote* quote = std::get_if<Quote>(&message);
    if(!quote)
        throw std::logic_error("Cannot handle non-quote message");

    orderBookService->setQuote(*quote);
    executionEngine->onQuoteReceived();
}

