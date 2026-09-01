#ifndef FRAME_HEADER_H
#define FRAME_HEADER_H

#include <cstdint>

namespace network {


#pragma pack(push, 1)
struct FrameHeader {
    uint16_t bodyLen;
    uint8_t msgType;
    uint8_t version;
};
#pragma pack(pop)

} // namespace network

#endif //FRAME_HEADER_H
