#include "MessageHandler.h"
#include <algorithm>
#include <cstring>
#include <new>
#include <utility>
#include "FrameHeader.h"

using network::FrameHeader;

MessageHandler::MessageHandler()
    : buffer(::operator new(BUFFER_SIZE)), size(0) { }

MessageHandler::~MessageHandler() {
    free();
}

MessageHandler::MessageHandler(MessageHandler&& other) {
    moveFrom(std::move(other));
}

MessageHandler& MessageHandler::operator=(MessageHandler&& other) {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

void MessageHandler::moveFrom(MessageHandler&& other) {
    buffer = other.buffer;
    size = other.size;
    other.buffer = nullptr;
    other.size = 0;
}

void MessageHandler::free() {
    ::operator delete(buffer);
}

MessagesRecieved MessageHandler::recieve(void* mem, size_t size) {

    if (this->size + size > BUFFER_SIZE)
        throw std::bad_alloc();

    if (dirtyStart != nullptr) {
        memcpy(buffer, dirtyStart, this->size);


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
    dirtyStart = std::max(dirtyStart, static_cast<void*>(static_cast<std::byte*>(buffer) + messageSize));
    this->size -= messageSize;
    return {static_cast<FrameHeader*>(buffer), others + 1};

}
