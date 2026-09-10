#ifndef ORDER_MESSAGE_SENDER_H
#define ORDER_MESSAGE_SENDER_H

#include "ClientConnection.h"
#include "OETypes.h"
#include "OESerialiser.h"
#include "SPSCQueue.hpp"
#include <thread>
#include <variant>


class OrderMessageSender {
private:
    ClientConnection* clientConnection;
    std::byte buffer[OESerialiser::MAX_FRAME_SIZE];
    SPSCQueue<std::variant<NewOrder, ExecReport>> messageQueue;

    std::thread runningThread;
    std::atomic<bool> shouldStop = false;

public: 
    OrderMessageSender(ClientConnection* con) : clientConnection(con), runningThread(&OrderMessageSender::run, this) {}
    ~OrderMessageSender();

    void send(NewOrder& order);
    void send(ExecReport& report);

private:

    void sendNewOrder(NewOrder& order);
    void sendExecReport(ExecReport& report);
    void run();


};






#endif // ORDER_MESSAGE_SENDER_H
