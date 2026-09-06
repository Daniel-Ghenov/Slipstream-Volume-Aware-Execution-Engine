#ifndef EXECUTION_ENGINE_H
#define EXECUTION_ENGINE_H

#include "OrderBookService.h"
#include "VWAPService.hpp"
#include "OETypes.h"
#include <cstdint>
#include <optional>



class ExecutionEngine {
private:
    uint32_t quotesReceived = 0;
    bool tradingStarted = false;
    bool isHalted = false;
    uint64_t maxQuantity;
    uint64_t participationCap;
    uint64_t vwapWindowMs;
    uint64_t bandBps;

    uint64_t timestampStarted;

    uint64_t tradedQuantity = 0;
    uint64_t cumulativeTradedQuantity = 0;

    VWAPService* vwapService;
    OrderBookService* orderBookService;

    uint64_t clientOrderIdCounter = 0;

public:

    ExecutionEngine(uint64_t maxQuantity, uint64_t participationCap, uint64_t vwapWindowMs, uint64_t bandBps, OrderBookService* orderBookService, VWAPService* vwapService);

    void onQuoteReceived();
    std::optional<NewOrder> handleTrade(const Trade& trade);
    uint64_t getExecutedQuantity() const { return tradedQuantity; }
    uint64_t getCumulativeExecutedQuantity() const { return cumulativeTradedQuantity; }

    void haltTrading() { isHalted = true; }
    void resumeTrading() { isHalted = false; }

private:
    bool isReadyToTrade();
    uint64_t getElapsedTimeMs() const;
    uint64_t getNow() const;
    bool shouldBuy(const Trade& trade) const;
    bool shouldSell(const Trade& trade) const;

    NewOrder handleBuy(const Trade& trade);
    NewOrder handleSell(const Trade& trade);
    NewOrder handleReject(const Trade& trade);

    static const size_t MIN_QUOTES_REQUIRED = 10;

};




#endif // EXECUTION_ENGINE_H
