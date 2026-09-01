#ifndef OE_SERIALISER_H
#define OE_SERIALISER_H

#include <algorithm>
#include "FrameHeader.h"
#include "OETypes.h"

class OESerialiser {
private:
    static constexpr size_t NEW_ORDER_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::NewOrderBody);
    static constexpr size_t EXEC_REPORT_MESSAGE_SIZE = sizeof(network::FrameHeader) + sizeof(network::ExecReportBody);
public:

    static size_t serialiseNewOrderMessage(void* buffer, network::NewOrderBody& body);
    static size_t serialiseExecReportMessage(void* buffer, network::ExecReportBody& body);

    static constexpr size_t MAX_FRAME_SIZE = std::max({NEW_ORDER_MESSAGE_SIZE, EXEC_REPORT_MESSAGE_SIZE});
private:
    static constexpr size_t PROTOCOL_VERSION = 1;
};


#endif //OE_SERIALISER_H
