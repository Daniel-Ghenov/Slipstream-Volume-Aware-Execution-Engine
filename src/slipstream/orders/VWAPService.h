#ifndef VWAP_SERVICE_H
#define VWAP_SERVICE_H

#include "GCMDTypes.h"
#include <cstdint>

class VWAPService {
private:
    unsigned __int128 totalPricedQuantity;
    uint64_t totalQuantity;

public:

    void handleTrade(Trade trade);
    uint64_t getVWAP() const;



};


#endif //VWAP_SERVICE_H
