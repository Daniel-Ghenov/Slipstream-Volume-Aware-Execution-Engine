#ifndef MESSAGE_DEMULTIPLEXER_H
#define MESSAGE_DEMULTIPLEXER_H

#include "GCMDTypes.h"
#include "MasterMessageHandler.h"
#include "MPSCQueue.hpp"
#include <thread>



class MessageDeMultiplexer {
private:

    MasterMessageHandler* masterHandler;
    MPSCQueue<MDMessage>* messageQueue;
    
    std::thread runningThread;
    std::atomic<bool> shouldStop = false;

public:
    ~MessageDeMultiplexer();
    MessageDeMultiplexer(MasterMessageHandler* masterHandler, MPSCQueue<MDMessage>* messageQueue): masterHandler(masterHandler), messageQueue(messageQueue) {}

    void start();
private:
    void run();
};


#endif // MESSAGE_DEMULTIPLEXER_H
