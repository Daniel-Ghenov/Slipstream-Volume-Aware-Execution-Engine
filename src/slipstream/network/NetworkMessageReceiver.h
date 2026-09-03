#ifndef NETWORK_MESSAGE_RECEIVER_H
#define NETWORK_MESSAGE_RECEIVER_H


#include "ClientConnection.h"
#include "NetworkMessageHandler.h"
#include "MessageHandler.h"
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <thread>

class NetworkMessageReceiver {
private:
    ClientConnection* clientConnection;
    MessageHandler* messageHandler;
    NetworkMessageHandler* networkMessageHandler;

    std::byte networkBuffer[MessageHandler::BUFFER_SIZE];
    std::thread runningThread;
    std::atomic<bool> shouldStop = false;
    uint64_t lastHeartbeat = 0;
    uint64_t heartbeatTimeoutMs;

public:
    NetworkMessageReceiver(ClientConnection*, MessageHandler*, NetworkMessageHandler*,
                            std::chrono::milliseconds receiveTimeout = std::chrono::milliseconds(50),
                            uint64_t heartbeatTimeoutMs = 100);

    NetworkMessageReceiver(const NetworkMessageReceiver&) = delete;
    NetworkMessageReceiver(NetworkMessageReceiver&&) = delete;
    NetworkMessageReceiver operator=(const NetworkMessageReceiver&) = delete;
    NetworkMessageReceiver operator=(NetworkMessageReceiver&&) = delete;

    ~NetworkMessageReceiver();

    void tryReceive();
    void start();
    void shutdown();

private:
    void run();
    bool heartbeatStale() const;
    static uint64_t getNow();
};


#endif // NETWORK_MESSAGE_RECEIVER_H
