#ifndef MESSAGE_DEMULTIPLEXER_H
#define MESSAGE_DEMULTIPLEXER_H

#include "GCMDTypes.h"
#include "InboundMessageQueue.h"
#include "MasterMessageHandler.h"
#include <thread>



class MessageDeMultiplexer {
private:

    MasterMessageHandler* masterHandler;
    InboundMessageQueue* messageQueue;

    std::thread runningThread;
    std::atomic<bool> shouldStop = false;

public:
    ~MessageDeMultiplexer();
    MessageDeMultiplexer(MasterMessageHandler* masterHandler, InboundMessageQueue* messageQueue): masterHandler(masterHandler), messageQueue(messageQueue) {}

    void start();
private:
    void run();
};


#endif // MESSAGE_DEMULTIPLEXER_H
