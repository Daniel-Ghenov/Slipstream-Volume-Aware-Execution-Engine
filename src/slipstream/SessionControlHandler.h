#ifndef SESSION_CONTROL_HANDLER_H
#define SESSION_CONTROL_HANDLER_H

#include "ExecutionEngine.h"
#include "NetworkMessageReceiver.h"
#include "ShutdownSignal.h"

#include <thread>
class SessionControlHandler {
private:

    SessionControlState currentState = SessionControlState::OPEN;
    ExecutionEngine* executionEngine;
    NetworkMessageReceiver* oeReceiver;
    NetworkMessageReceiver* mdReceiver;
    ShutdownSignal* shutdownSignal;

    std::thread sessionThread;
    std::atomic<bool> shouldStop{false};


public:
    SessionControlHandler(ExecutionEngine* executionEngine, NetworkMessageReceiver* oeReceiver, NetworkMessageReceiver* mdReceiver, ShutdownSignal* shutdownSignal)
        : executionEngine(executionEngine), oeReceiver(oeReceiver), mdReceiver(mdReceiver), shutdownSignal(shutdownSignal) {}

    ~SessionControlHandler();

    void startSession();

private:
    void runSession();
    void handleHalt();
    void handleOpen();
    void handleClose();

};







#endif //SESSION_CONTROL_HANDLER_H
