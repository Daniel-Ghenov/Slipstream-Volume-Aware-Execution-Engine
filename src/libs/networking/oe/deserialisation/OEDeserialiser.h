#ifndef OE_DESERIALISER_H
#define OE_DESERIALISER_H

#include <algorithm>
#include <cstdint>
#include "OETypes.h"

class OEDeserialiser {
private:
    static constexpr size_t NEW_ORDER_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(NewOrderBody);
    static constexpr size_t EXEC_REPORT_MESSAGE_SIZE = sizeof(FrameHeader) + sizeof(ExecReportBody);
public:

    NewOrderBody getNewOrderMessage(void* buffer);
    ExecReportBody getExecReportMessage(void* buffer);
    size_t getBodySize(void* buffer);
    OEMessageType getMessageType(void* buffer);
    bool isCompleteHeader(size_t size);

    static constexpr size_t MAX_FRAME_SIZE = std::max({NEW_ORDER_MESSAGE_SIZE, EXEC_REPORT_MESSAGE_SIZE});
private:
    static constexpr size_t PROTOCOL_VERSION = 1;
};


#endif //OE_DESERIALISER_H
