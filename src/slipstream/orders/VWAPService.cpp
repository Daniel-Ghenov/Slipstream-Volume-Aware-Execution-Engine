#include "VWAPService.hpp"
#include <chrono>


uint64_t VWAPService::getVWAP() {
    removeOldTrades(getNow());

    if (totalQuantity == 0) {
        return 0;
    }
    
    return totalPricedQuantity / totalQuantity;
}


void VWAPService::addTrade(Trade trade) {
    trades[head] = trade;
    head = (head + 1) % BUFFER_SIZE;

    totalQuantity += trade.quantity;
    totalPricedQuantity += static_cast<unsigned __int128>(trade.quantity) * trade.price;

}

void VWAPService::removeOldTrades(uint64_t currentTimestamp) {

    while (head != tail && totalQuantity > 0) {
        Trade& oldestTrade = trades[tail];
        if (currentTimestamp - oldestTrade.timestampNs <= rollingWindowSizeMs) {
            break;
        }

        totalQuantity -= oldestTrade.quantity;
        totalPricedQuantity -= static_cast<unsigned __int128>(oldestTrade.quantity) * oldestTrade.price;

        tail = (tail + 1) % BUFFER_SIZE;
    }

}

void VWAPService::removeOldestTrade() {
    if (head == tail && totalQuantity == 0) {
        return;
    }

    Trade& oldestTrade = trades[tail];
    totalQuantity -= oldestTrade.quantity;
    totalPricedQuantity -= static_cast<unsigned __int128>(oldestTrade.quantity) * oldestTrade.price;

    tail = (tail + 1) % BUFFER_SIZE;
}

uint64_t VWAPService::getNow() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return 0;
}

