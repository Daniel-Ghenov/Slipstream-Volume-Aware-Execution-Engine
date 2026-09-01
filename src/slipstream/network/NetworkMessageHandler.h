#ifndef NETWORK_MESSAGE_HANDLER_H
#define NETWORK_MESSAGE_HANDLER_H

#include "GCMDTypes.h"

class NetworkMessageHandler {
public:

    virtual ~NetworkMessageHandler() = default;
    virtual void onMessage(const MDMessage& message) = 0;

};




#endif // NETWORK_MESSAGE_HANDLER_H
