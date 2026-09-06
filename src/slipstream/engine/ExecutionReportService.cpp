#include "ExecutionReportService.h"
#include <iostream>


void ExecutionReportService::printExecutionReport() {
// === SLIPSTREAM EXECUTION REPORT ===
// symbol              SYNTH1
// market qty          50000
// executed qty        47300   (94.60%)
// avg fill price      101.2438
// session VWAP        101.2701
// slippage vs VWAP    -2.60 bps   (favorable)
// participation       11.83%  (cap 15.00%)
// tick-to-order p50   1.9 us
// tick-to-order p99   6.4 us
// tick-to-order p99.9 21.7 us

    std::cout << "=== SLIPSTREAM EXECUTION REPORT ===" << std::endl;
    std::cout << "symbol              " << std::string_view(symbol, 12) << std::endl;

    uint64_t cumulativeMarketVolume = executionEngine->getCumulativeExecutedQuantity();
    uint64_t exec = executionEngine->getExecutedQuantity();

    std::cout << "market qty          " << cumulativeMarketVolume << std::endl;
    std::cout << "executed qty        " << exec << "   (" << (exec * 100.0 / cumulativeMarketVolume) << "%)" << std::endl;


}


