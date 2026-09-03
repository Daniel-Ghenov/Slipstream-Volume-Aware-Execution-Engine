#include "NetworkMessageReceiver.h"
#include "GCMDDeserialiser.h"
#include <atomic>
#include <functional>
#include <iostream>


NetworkMessageReceiver::NetworkMessageReceiver(ClientConnection* con, MessageHandler* mh, NetworkMessageHandler* nmh,
                                                std::chrono::milliseconds receiveTimeout, uint64_t heartbeatTimeoutMs):
                                                clientConnection(con),
                                                messageHandler(mh),
                                                networkMessageHandler(nmh),
                                                heartbeatTimeoutMs(heartbeatTimeoutMs) {
    clientConnection->setReceiveTimeout(receiveTimeout);
    lastHeartbeat = getNow();
}

void NetworkMessageReceiver::tryReceive() {

    size_t bytesRecv = clientConnection->receiveBuffer(networkBuffer, MessageHandler::BUFFER_SIZE);

    if (bytesRecv == 0)
        return;

    auto [ptr, count] = messageHandler->recieve(networkBuffer, bytesRecv);

    for(size_t i{0uz}; i < count; ++i) {
        MDMessage message = GCMDDeserialiser::deserialiseMessage(ptr);
        Heartbeat* hb = std::get_if<Heartbeat>(&message);
        if (hb && hb->timestamp > lastHeartbeat) {
            lastHeartbeat = hb->timestamp;
        }

        networkMessageHandler->onMessage(message);
        ptr = static_cast<std::byte*>(ptr) + GCMDDeserialiser::getMessageSize(ptr);
    }
}

NetworkMessageReceiver::~NetworkMessageReceiver() {
    shutdown();
    runningThread.join();
}

void NetworkMessageReceiver::start() {
    if (runningThread.joinable())
        return;
    runningThread = std::thread(std::mem_fn(&NetworkMessageReceiver::run), this);
}

void NetworkMessageReceiver::shutdown() {
    clientConnection->shutdown();
    shouldStop.store(true, std::memory_order_release);
}

void NetworkMessageReceiver::run() {
    while(!shouldStop.load(std::memory_order_acquire)) {
        try {
            tryReceive();

            if (heartbeatStale()) {
                std::cerr << "No heartbeat in over " << heartbeatTimeoutMs << "ms, closing connection" << std::endl;
                shutdown();
                return;
            }
        }
        catch (std::out_of_range& err) {
            if (shouldStop.load(std::memory_order_acquire))
                return;

            std::cerr << "Connection closed by peer" << std::endl;
            return;

        }
        catch(std::exception& err) {
            std::cerr << "Error while receiving messages: " << err.what() << std::endl;
            return;
        }
        catch(...) {
            std::cerr << "Unkown exception occurred during receiving messages" << std::endl;
            return;
        }
    }
}

bool NetworkMessageReceiver::heartbeatStale() const {
    return getNow() - lastHeartbeat > heartbeatTimeoutMs;
}

uint64_t NetworkMessageReceiver::getNow() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}
