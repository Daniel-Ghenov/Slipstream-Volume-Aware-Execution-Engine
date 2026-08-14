#include "ClientConnection.h"
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 


ClientConnection::ClientConnection(const std::string& ip, int port) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    int connected = connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (connected == -1)
        throw std::runtime_error("Could not connect to server");
}

size_t ClientConnection::receiveBuffer(void* buffer, size_t bufferSize) {

    if (fd == -1) {
        throw std::logic_error("Connection not opened");
    }

    ssize_t n = recv(fd, buffer, bufferSize, 0);

    while (n < 0 && errno == EINTR) {
        n = recv(fd, buffer, bufferSize, 0);
    }

    if (n < 0)
        throw std::runtime_error(std::string("recv failed: ") + std::strerror(errno));

    if (n == 0)
        throw std::out_of_range("connection closed by peer");

    return static_cast<size_t>(n);
}

size_t ClientConnection::sendBuffer(void* buffer, size_t bufferSize) {

    if (fd == -1) {
        throw std::logic_error("Connection not opened");
    }

    ssize_t n = ::send(fd, buffer, bufferSize, MSG_NOSIGNAL);

    if (n < 0)
        throw std::runtime_error(std::string("send failed: ") + std::strerror(errno));

    if (n == 0)
        throw std::out_of_range("connection closed by peer");

    return static_cast<size_t>(n);
}

ClientConnection::~ClientConnection() {
    free();
}

ClientConnection::ClientConnection(ClientConnection&& other) {
    moveFrom(std::move(other));
}

ClientConnection& ClientConnection::operator=(ClientConnection&& other) {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

void ClientConnection::moveFrom(ClientConnection&& other) {
    fd = other.fd;
    other.fd = -1;
}

void ClientConnection::free() {
    close(fd);
}
