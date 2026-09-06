#ifndef EXECUTION_REPORT_SERVICE_H
#define EXECUTION_REPORT_SERVICE_H

#include "ExecutionEngine.h"
#include "VWAPService.hpp"
#include <cstdint>

class ExecutionReportService {
    char symbol[12];
    uint64_t marketParticipationCap;
    VWAPService* vwapService;
    ExecutionEngine* executionEngine;

public:

    ExecutionReportService(const char symbol[12], uint64_t marketParticipationCap, VWAPService* vwapService, ExecutionEngine* executionEngine)
        : marketParticipationCap(marketParticipationCap), vwapService(vwapService) , executionEngine(executionEngine) {
        memcpy(this->symbol, symbol, 12);
    }

    void printExecutionReport();

};

#endif // EXECUTION_REPORT_SERVICE_H
