#include "MessageHandler.h"
#include <cstring>
#include <new>
#include <utility>

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
    size_t currSize = this->size;
    if (currSize + size > BUFFER_SIZE)
        throw std::bad_alloc();

    memcpy(static_cast<std::byte*>(buffer) + currSize, mem, size);
    this->size += size;

    return getMessages(buffer, size);
}

MessagesRecieved MessageHandler::getMessages(void* buffer, size_t size) {

    if (size < sizeof(FrameHeader))
        return {nullptr, 0};

    FrameHeader header = *(static_cast<FrameHeader*>(buffer));

    if(header.bodyLen > size)
        return {nullptr, 0};
    size_t messageSize = sizeof(FrameHeader) + header.bodyLen;
    auto [_ns, others] = getMessages(static_cast<std::byte*>(buffer) + messageSize, size - messageSize);
    this->size -= messageSize;
    return {static_cast<FrameHeader*>(buffer), others + 1};

}
