#include "QuoteMessageHandler.h"
#include "GCMDTypes.h"
#include <stdexcept>

void QuoteMessageHandler::onMessage(const MDMessage& message) {

    const Quote* quote = std::get_if<Quote>(&message);
    if(!quote)
        throw std::logic_error("Cannot handle non-quote message");

    orderBookService->setQuote(*quote);
    executionEngine->onQuoteReceived();
}

bool QuoteMessageHandler::canHandleMessage(const MDMessage& message) {
    return std::holds_alternative<Quote>(message);
}

