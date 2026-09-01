#include "ServerConnection.h"
#include <cerrno>
#include <cstring>
#include <stdexcept>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


ServerConnection::ServerConnection(const std::string& ip, int port) {
    addrinfo hints{};
    addrinfo* res;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in the wildcard address when ip is empty

    const char* node = ip.empty() ? nullptr : ip.c_str();
    int status = getaddrinfo(node, std::to_string(port).c_str(), &hints, &res);

    if (status != 0)
        throw std::runtime_error(std::string("getaddrinfo failed: ") + gai_strerror(status));

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (fd == -1) {
        freeaddrinfo(res);
        throw std::logic_error("Error, cannot create socket");
    }

    int reuse = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int bound = bind(fd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    if (bound == -1)
        throw std::runtime_error(std::string("bind failed: ") + std::strerror(errno));

    if (listen(fd, SOMAXCONN) == -1)
        throw std::runtime_error(std::string("listen failed: ") + std::strerror(errno));
}

ClientConnection ServerConnection::accept() {
    int clientFd = ::accept(fd, nullptr, nullptr);

    while (clientFd < 0 && errno == EINTR) {
        clientFd = ::accept(fd, nullptr, nullptr);
    }

    if (clientFd < 0)
        throw std::runtime_error(std::string("accept failed: ") + std::strerror(errno));

    return ClientConnection(clientFd);
}

ServerConnection::~ServerConnection() {
    free();
}

ServerConnection::ServerConnection(ServerConnection&& other) {
    moveFrom(std::move(other));
}

ServerConnection& ServerConnection::operator=(ServerConnection&& other) {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

void ServerConnection::moveFrom(ServerConnection&& other) {
    fd = other.fd;
    other.fd = -1;
}

void ServerConnection::free() {
    close(fd);
}
