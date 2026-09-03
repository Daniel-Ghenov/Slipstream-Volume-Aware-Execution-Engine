#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <cstddef>

struct MessagesRecieved {
    void* start;
    size_t messagesCount;
};


class MessageHandler {
public:
    static constexpr size_t BUFFER_SIZE = 2048;

    //using recieve invalidates any memory returned from the function.
    //make sure to copy any data from the returned memory before invoking recieve() again
    MessagesRecieved recieve(void* buffer, size_t size);

private:
    std::byte buffer[BUFFER_SIZE];
    size_t size = 0;
    size_t dirtyStart = 0;

    MessagesRecieved getMessages(void* buffer, size_t size);
};

#endif //MESSAGE_HANDLER_H
