#include "OESerialiser.h"
#include "OETypes.h"

size_t OESerialiser::serialiseNewOrderMessage(void* buffer, NewOrderBody& body) {
    FrameHeader header = {sizeof(NewOrderBody), oeMessageTypeToNum(OEMessageType::NEW_ORDER), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(NewOrderBody));
    return NEW_ORDER_MESSAGE_SIZE;
}
size_t OESerialiser::serialiseExecReportMessage(void* buffer, ExecReportBody& body) {
    FrameHeader header = {sizeof(ExecReportBody), oeMessageTypeToNum(OEMessageType::EXEC_REPORT), PROTOCOL_VERSION};
    memcpy(buffer, &header, sizeof(FrameHeader));
    memcpy(static_cast<std::byte*>(buffer) + sizeof(FrameHeader), &body, sizeof(ExecReportBody));
    return EXEC_REPORT_MESSAGE_SIZE;
}
