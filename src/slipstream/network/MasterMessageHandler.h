#ifndef MASTER_MESSAGE_HANDLER_H
#define MASTER_MESSAGE_HANDLER_H

#include "MessageHandler.h"
#include <vector>

class MasterMessageHandler: public MessageHandler {
private:
    std::vector<MessageHandler*> handlers;
public:
    MasterMessageHandler() = default;
    MasterMessageHandler(std::initializer_list<MessageHandler*> handlers) : handlers(handlers) {}

    ~MasterMessageHandler() = default;
    void onMessage(const MDMessage& message) override;
    bool canHandleMessage(const MDMessage& message) override;
};

#endif // MASTER_MESSAGE_HANDLER_H
