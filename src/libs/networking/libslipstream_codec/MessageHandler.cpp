#include "MessageHandler.h"
#include <algorithm>
#include <cstring>
#include <new>
#include "FrameHeader.h"

using network::FrameHeader;

MessagesRecieved MessageHandler::recieve(void* mem, size_t size) {

    if (this->size + size > BUFFER_SIZE)
        throw std::bad_alloc();

    if (dirtyStart != 0) {
        memcpy(buffer, buffer + dirtyStart, this->size);
        dirtyStart = 0;
    }

    memcpy(static_cast<std::byte*>(buffer) + this->size, mem, size);
    this->size += size;

    return getMessages(buffer, this->size);
}

MessagesRecieved MessageHandler::getMessages(void* buffer, size_t size) {

    if (size < sizeof(FrameHeader))
        return {nullptr, 0};

    FrameHeader header = *(static_cast<FrameHeader*>(buffer));

    if(header.bodyLen + sizeof(FrameHeader) > size)
        return {nullptr, 0};
    size_t messageSize = sizeof(FrameHeader) + header.bodyLen;
    auto [_ns, others] = getMessages(static_cast<std::byte*>(buffer) + messageSize, size - messageSize);
    const size_t consumedOffset = (static_cast<std::byte*>(buffer) - this->buffer) + messageSize;
    dirtyStart = std::max(dirtyStart, consumedOffset);
    this->size -= messageSize;
    return {static_cast<FrameHeader*>(buffer), others + 1};

}
