#ifndef ORDER_MESSAGE_SENDER_H
#define ORDER_MESSAGE_SENDER_H

#include "ClientConnection.h"
#include "OETypes.h"
#include "OESerialiser.h"

class OrderMessageSender {
private:
    ClientConnection* clientConnection;
    std::byte buffer[OESerialiser::MAX_FRAME_SIZE];

public: 

    OrderMessageSender(ClientConnection* con) : clientConnection(con) {}

    void send(NewOrder& order);
    void send(ExecReport& report);

};






#endif // ORDER_MESSAGE_SENDER_H
