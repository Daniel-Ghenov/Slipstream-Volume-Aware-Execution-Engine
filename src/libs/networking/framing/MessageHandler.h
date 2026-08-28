#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <cstddef>
#include "FrameHeader.h"

struct MessagesRecieved {
    FrameHeader* start;
    size_t messagesCount;
};

// Protocol-agnostic: reassembles frames from a byte stream using only the
// shared FrameHeader (bodyLen), so the same instance can be reused for any
// protocol built on top of this framing (GCMD, OE, ...). Decoding a frame's
// body once you have a pointer to it is the corresponding Deserialiser's job.
class MessageHandler {
private:
    void* buffer;
    size_t size;

    // Upper bound on a single frame's size: header plus the largest body
    // that bodyLen (uint16_t) could ever describe, regardless of protocol.
    static constexpr size_t BUFFER_SIZE = sizeof(FrameHeader) + UINT16_MAX;

public:
    MessageHandler();
    ~MessageHandler();

    MessageHandler(const MessageHandler&) = delete;
    MessageHandler& operator=(const MessageHandler&) = delete;

    MessageHandler(MessageHandler&& other);
    MessageHandler& operator=(MessageHandler&& other);

    //using recieve invalidates any memory returned from the function.
    //make sure to copy any data from the returned memory before invoking recieve() again
    MessagesRecieved recieve(void* buffer, size_t size);

private:
    void moveFrom(MessageHandler&& other);
    void free();

    MessagesRecieved getMessages(void* buffer, size_t size);
};

#endif //MESSAGE_HANDLER_H
