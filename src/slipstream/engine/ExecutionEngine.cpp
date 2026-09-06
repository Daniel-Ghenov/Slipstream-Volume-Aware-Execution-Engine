#include "ExecutionEngine.h"
#include <chrono>
#include <cstdint>


ExecutionEngine::ExecutionEngine(uint64_t maxQuantity, uint64_t participationCap, uint64_t vwapWindowMs, uint64_t bandBps, OrderBookService* orderBookService, VWAPService* vwapService)
    : quotesReceived(0), maxQuantity(maxQuantity), participationCap(participationCap), vwapWindowMs(vwapWindowMs), bandBps(bandBps) , orderBookService(orderBookService), vwapService(vwapService) {
}


void ExecutionEngine::onQuoteReceived() {
    quotesReceived++;
    if ((quotesReceived == 1) && !tradingStarted) [[unlikely]] {
        timestampStarted = getNow();
    }
}


bool ExecutionEngine::isReadyToTrade() {
    if (tradingStarted) [[likely]]
        return true;
    bool ready = quotesReceived >= MIN_QUOTES_REQUIRED && getElapsedTimeMs() >= vwapWindowMs;
    if (ready)
        tradingStarted = true;      
    return ready;
}


uint64_t ExecutionEngine::getElapsedTimeMs() const  {

    return getNow() - timestampStarted;
}

uint64_t ExecutionEngine::getNow() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool ExecutionEngine::shouldBuy(const Trade& trade) const {
    uint64_t currentPrice = trade.price;
    uint64_t vwap = vwapService->getVWAP();
    uint64_t upperBound = vwap * (10000 - bandBps) / 10000;
    bool shouldBuy = currentPrice <= upperBound;
    bool canBuy = (cumulativeTradedQuantity + trade.quantity) <= participationCap && (tradedQuantity + trade.quantity) <= maxQuantity;
    
    return shouldBuy && canBuy;
}

bool ExecutionEngine::shouldSell(const Trade& trade) const {
    uint64_t currentPrice = trade.price;
    uint64_t vwap = vwapService->getVWAP();
    uint64_t lowerBound = vwap * (10000 + bandBps) / 10000;
    bool shouldBuy = currentPrice >= lowerBound;
    bool canBuy = tradedQuantity >= trade.quantity;
    
    return shouldBuy && canBuy;
}

std::optional<NewOrder> ExecutionEngine::handleTrade(const Trade& trade) {
    if (!isReadyToTrade()) {
        return handleReject(trade);
    }

    if (isHalted) {
        handleReject(trade);
        return std::nullopt;
    }

    if (trade.aggressor == TradeAggressor::BUY) {
        if (shouldBuy(trade)) {
            return handleBuy(trade);
        }
    } else if (trade.aggressor == TradeAggressor::SELL) {
        if (shouldSell(trade)) {
            return handleSell(trade);
        }
    }
    return handleReject(trade);
}

NewOrder ExecutionEngine::handleBuy(const Trade& trade) {
    tradedQuantity += trade.quantity;
    cumulativeTradedQuantity += trade.quantity;
    uint64_t tsNs = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    return NewOrder(clientOrderIdCounter++, trade.symbol, OrderStatus::ACCEPTED, tsNs, trade.id, OrderSide::BUY, trade.quantity, trade.price);
}

NewOrder ExecutionEngine::handleSell(const Trade& trade) {
    tradedQuantity += trade.quantity;
    cumulativeTradedQuantity += trade.quantity;
    uint64_t tsNs = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    return NewOrder(clientOrderIdCounter++, trade.symbol, OrderStatus::ACCEPTED, tsNs, trade.id, OrderSide::SELL, trade.quantity, trade.price);
}

NewOrder ExecutionEngine::handleReject(const Trade& trade) {
    cumulativeTradedQuantity += trade.quantity;
    uint64_t tsNs = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    OrderSide side = trade.aggressor == TradeAggressor::BUY ? OrderSide::BUY : OrderSide::SELL;
    return NewOrder(clientOrderIdCounter++, trade.symbol, OrderStatus::REJECTED, tsNs, trade.id, side, trade.quantity, trade.price);
}
