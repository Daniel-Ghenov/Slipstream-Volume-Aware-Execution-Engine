#ifndef NETWORK_MESSAGE_HANDLER_H
#define NETWORK_MESSAGE_HANDLER_H

#include "GCMDTypes.h"



class MessageHandler {
public:

    virtual ~MessageHandler() = default;
    virtual void onMessage(const MDMessage& message) = 0;
    virtual bool canHandleMessage(const MDMessage& message) = 0;
};



#endif // NETWORK_MESSAGE_HANDLER_H
