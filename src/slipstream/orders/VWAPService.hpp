#ifndef VWAP_SERVICE_HPP
#define VWAP_SERVICE_HPP

#include "GCMDTypes.h"
#include <cstdint>
#include <stdexcept>

enum class OverflowPolicy {
    REJECT_NEW,
    OVERWRITE_OLDEST,
    THROW
};

class VWAPService {
private:
    static constexpr size_t BUFFER_SIZE = 1024;

    unsigned __int128 totalPricedQuantity = 0;
    uint64_t totalQuantity = 0;
    uint64_t rollingWindowSizeMs;
    Trade trades[BUFFER_SIZE];
    size_t head = 0;
    size_t tail = 0;

public:
    VWAPService(uint64_t rollingWindowSizeMs): rollingWindowSizeMs(rollingWindowSizeMs) {}
    template<OverflowPolicy Policy>
    void handleTrade(Trade trade);
    uint64_t getVWAP();

private:
    void addTrade(Trade trade);
    void removeOldTrades(uint64_t currentTimestamp);
    void removeOldestTrade();

    uint64_t getNow() const; 
};

template<OverflowPolicy Policy>
void VWAPService::handleTrade(Trade trade) {
    removeOldTrades(trade.timestampNs);
    if (head == tail && totalQuantity > 0) {
        if constexpr (Policy == OverflowPolicy::REJECT_NEW) {
            return;
        } else if constexpr (Policy == OverflowPolicy::OVERWRITE_OLDEST) {
            removeOldestTrade();
        } else if constexpr (Policy == OverflowPolicy::THROW) {
            throw std::runtime_error("Buffer overflow");
        }
    }
    addTrade(trade);
}

#endif //VWAP_SERVICE_HPP
