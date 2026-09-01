#include "ClientConnection.h"
#include <cerrno>
#include <chrono>
#include <cstring>
#include <stdexcept>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


ClientConnection::ClientConnection(const std::string& ip, int port) {
    sockaddr_in addr{};
    addrinfo hints;
    addrinfo *res;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int status = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &res);

    if (status != 0)
        throw std::runtime_error(std::string("getaddrinfo failed: ") + gai_strerror(status));

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (fd == -1)
        throw std::logic_error(" Error, cannot create socket");


    int connected = connect(fd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if (connected == -1)
        throw std::runtime_error("Could not connect to server");
}

ClientConnection::ClientConnection(int fd) : fd(fd) { }

size_t ClientConnection::receiveBuffer(void* buffer, size_t bufferSize) {

    if (fd == -1) {
        throw std::logic_error("Connection not opened");
    }

    ssize_t n = recv(fd, buffer, bufferSize, 0);

    while (n < 0 && errno == EINTR) {
        n = recv(fd, buffer, bufferSize, 0);
    }

    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return 0;

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

void ClientConnection::setReceiveTimeout(std::chrono::milliseconds timeout) {
    timeval tv{};
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
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


void ClientConnection::shutdown() {
    ::shutdown(fd, SHUT_RDWR);
}
