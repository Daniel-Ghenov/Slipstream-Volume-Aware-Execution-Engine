#ifndef FRAME_HEADER_H
#define FRAME_HEADER_H

#include <cstdint>

// Shared by every wire protocol on top of this transport (GCMD, OE, ...).
// Each protocol defines its own message-type numbering for msgType.
#pragma pack(push, 1)
struct FrameHeader {
    uint16_t bodyLen;
    uint8_t msgType;
    uint8_t version;
};
#pragma pack(pop)

#endif //FRAME_HEADER_H
