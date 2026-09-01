#include "OEDeserialiser.h"

using namespace network;

NewOrderBody OEDeserialiser::getNewOrderMessage(void* buffer) {
    NewOrderBody* body = reinterpret_cast<NewOrderBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return *body;
}
ExecReportBody OEDeserialiser::getExecReportMessage(void* buffer) {
    ExecReportBody* body = reinterpret_cast<ExecReportBody*>(static_cast<std::byte*>(buffer) + sizeof(FrameHeader));
    return *body;
}

size_t OEDeserialiser::getBodySize(void* buffer) {
    FrameHeader* header = static_cast<FrameHeader*>(buffer);
    return header->bodyLen;
}
OEMessageType OEDeserialiser::getMessageType(void* buffer) {
    FrameHeader* header = static_cast<FrameHeader*>(buffer);
    return oeMessageTypeFromNum(header->msgType);
}

bool OEDeserialiser::isCompleteHeader(size_t size) {
    return size >= sizeof(FrameHeader);
}
