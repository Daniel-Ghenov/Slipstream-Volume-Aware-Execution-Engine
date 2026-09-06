#include "SessionControlHandler.h"
#include <iostream>


SessionControlHandler::~SessionControlHandler() {
    if (sessionThread.joinable()) {
        sessionThread.detach();
    }
}

void SessionControlHandler::startSession() {
    if (sessionThread.joinable()) {
        return;
    }
    sessionThread = std::thread(&SessionControlHandler::runSession, this);
}


void SessionControlHandler::runSession() {
    while (!shouldStop.load(std::memory_order_acquire)) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "HALT") {
            handleHalt();
        } else if (input == "OPEN") {
            handleOpen();
        } else if (input == "CLOSE") {
            std::cout << "CLOSE command received. Stopping trading." << std::endl;
            handleClose();
        } else {
            std::cout << "Unknown command: " << input << std::endl;
        }
    }
}


void SessionControlHandler::handleHalt() {
    if (currentState != SessionControlState::OPEN) {
        std::cout << "Cannot halt session. Current state is not OPEN." << std::endl;
        return;
    }
    executionEngine->haltTrading();
    currentState = SessionControlState::HALT;
    std::cout << "HALT command received. Stopping session." << std::endl;
}

void SessionControlHandler::handleOpen() {
    if (currentState != SessionControlState::HALT) {
        std::cout << "Cannot open session. Current state is not HALT." << std::endl;
        return;
    }
    executionEngine->resumeTrading();
    currentState = SessionControlState::OPEN;
    std::cout << "OPEN command received. Starting trading." << std::endl;
}

void SessionControlHandler::handleClose() {
    if (currentState == SessionControlState::CLOSE) {
        std::cout << "Session is already closed." << std::endl;
        return;
    }
    currentState = SessionControlState::CLOSE;
    shouldStop.store(true, std::memory_order_release);
    oeReceiver->shutdown();
    mdReceiver->shutdown();
    shutdownSignal->trigger();
    std::cout << "Session closed." << std::endl;
}
