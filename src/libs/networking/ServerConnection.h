#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>
#include "ClientConnection.h"

class ServerConnection {
private:
    int fd;
public:

    ServerConnection(const std::string& ip, int port);
    ~ServerConnection();

    ClientConnection accept();

    ServerConnection(const ServerConnection&) = delete;
    ServerConnection& operator=(const ServerConnection&) = delete;

    ServerConnection(ServerConnection&& other);
    ServerConnection& operator=(ServerConnection&& other);

private:
    void moveFrom(ServerConnection&& other);
    void free();
};

#endif // SERVER_CONNECTION_H
