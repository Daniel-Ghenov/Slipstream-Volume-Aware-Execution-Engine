#include "MasterMessageHandler.h"


void MasterMessageHandler::onMessage(const MDMessage& message) {
    for (auto handler : handlers) {
        if (handler->canHandleMessage(message)) {
            handler->onMessage(message);
            return;
        }
    }
}

bool MasterMessageHandler::canHandleMessage(const MDMessage& message) {
    for (auto handler : handlers) {
        if (handler->canHandleMessage(message)) {
            return true;
        }
    }
    return false;
}
