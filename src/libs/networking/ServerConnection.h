#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>

class ServerConnection {
private:
    int fd;
public:

    ServerConnection(const std::string& ip, int port);
    ~ServerConnection();

    size_t receiveBuffer(void* buffer, size_t bufferSize);
    size_t sendBuffer(void* buffer, size_t bufferSize);

    ServerConnection(const ServerConnection&) = delete;
    ServerConnection& operator=(const ServerConnection&) = delete;

    ServerConnection(ServerConnection&& other);
    ServerConnection& operator=(ServerConnection&& other);

private:
    void moveFrom(ServerConnection&& other);
    void free();
};

#endif // SERVER_CONNECTION_H
