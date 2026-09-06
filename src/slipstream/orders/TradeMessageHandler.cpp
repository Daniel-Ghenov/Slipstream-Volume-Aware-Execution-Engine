#include "TradeMessageHandler.h"
#include "GCMDTypes.h"
#include <stdexcept>

void TradeMessageHandler::onMessage(const MDMessage& message) {

    const Trade* trade = std::get_if<Trade>(&message);
    if(!trade)
        throw std::logic_error("Cannot handle non-quote message");

    vwapService->handleTrade<OverflowPolicy::OVERWRITE_OLDEST>(*trade);
    NewOrder newOrder = executionEngine->handleTrade(*trade);
    orderMessageSender->send(newOrder);

}

bool TradeMessageHandler::canHandleMessage(const MDMessage& message) {
    return std::holds_alternative<Trade>(message);
}
