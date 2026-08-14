#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <string>
class ClientConnection {
private:
    int fd;
public:

    ClientConnection(const std::string& ip, int port);
    ~ClientConnection();

    size_t receiveBuffer(void* buffer, size_t bufferSize);
    size_t sendBuffer(void* buffer, size_t bufferSize);

    ClientConnection(const ClientConnection&) = delete;
    ClientConnection& operator=(const ClientConnection&) = delete;

    ClientConnection(ClientConnection&& other);
    ClientConnection& operator=(ClientConnection&& other);

private:
    void moveFrom(ClientConnection&& other);
    void free();
};

#endif // CLIENT_CONNECTION_H
