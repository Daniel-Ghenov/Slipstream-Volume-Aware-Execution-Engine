#include "VWAPService.h"

void VWAPService::handleTrade(Trade trade) {
    totalQuantity += trade.quantity;
    totalPricedQuantity += trade.quantity * trade.price;
}
uint64_t VWAPService::getVWAP() const {
    return totalPricedQuantity / totalQuantity;
}
