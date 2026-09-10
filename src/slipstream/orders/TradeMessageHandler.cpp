#include "TradeMessageHandler.h"
#include "GCMDTypes.h"
#include <stdexcept>

void TradeMessageHandler::onMessage(const MDMessage& message) {

    const Trade* trade = std::get_if<Trade>(&message);
    if(!trade)
        throw std::logic_error("Cannot handle non-quote message");

    vwapService->handleTrade<OverflowPolicy::OVERWRITE_OLDEST>(*trade);
    std::optional<NewOrder> newOrder = executionEngine->handleTrade(*trade);
    if (newOrder.has_value())
        orderMessageSender->send(newOrder.value());

}

bool TradeMessageHandler::canHandleMessage(const MDMessage& message) {
    return std::holds_alternative<Trade>(message);
}
