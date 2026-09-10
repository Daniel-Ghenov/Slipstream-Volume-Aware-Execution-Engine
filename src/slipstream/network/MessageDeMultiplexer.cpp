#include "MessageDeMultiplexer.h"

MessageDeMultiplexer::~MessageDeMultiplexer() {
    shouldStop = true;
    if (runningThread.joinable())
        runningThread.join();
}


void MessageDeMultiplexer::start() {
    runningThread = std::thread(&MessageDeMultiplexer::run, this);
}


void MessageDeMultiplexer::run() {
    while(!shouldStop) {
        while (messageQueue->empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }   
        MDMessage message = messageQueue->pop();
        masterHandler->onMessage(message);
    }
}


