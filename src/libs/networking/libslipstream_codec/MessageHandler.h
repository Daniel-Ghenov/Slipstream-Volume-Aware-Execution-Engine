#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <cstddef>

struct MessagesRecieved {
    void* start;
    size_t messagesCount;
};


class MessageHandler {
private:
    void* buffer;
    size_t size;
    void* dirtyStart = 0;



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

    static constexpr size_t BUFFER_SIZE = 2048;

private:
    void moveFrom(MessageHandler&& other);
    void free();

    MessagesRecieved getMessages(void* buffer, size_t size);
};

#endif //MESSAGE_HANDLER_H
