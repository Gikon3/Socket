#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <vector>
#include "Server.h"
#include "Exception.h"

namespace Socket
{

Server::Server()
{
}

Server::Server(unsigned int addr, unsigned short port) :
    address_(addr), port_(port)
{
}

Server::Server(Address address) :
    Server(address.num[0] << 24 | address.num[1] << 16 | address.num[2] << 8 | address.num[3], address.port)
{
}

Server::~Server()
{
    close();
}

Server::Server(Server&& other)
{
    operator=(std::move(other));
}

Server& Server::operator=(Server&& other)
{
    std::swap(sockServer_, other.sockServer_);
    return *this;
}

void Server::bind()
{
    if (!isClose())
        throw Exception::AlreadyBound{port_};

    sockServer_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockServer_ < 0)
        throw Exception::Create{strerror(errno)};

    const int enable = 1;
    if (::setsockopt(sockServer_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
        throw Exception::Option{strerror(errno)};

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port_);
    saddr.sin_addr.s_addr = htonl(address_);
    if (::bind(sockServer_, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) < 0)
        throw Exception::Bind{strerror(errno)};

    if (::listen(sockServer_, 1) != 0)
        throw Exception::Listen{strerror(errno)};
}

void Server::bind(unsigned int addr, unsigned short port)
{
    if (!isClose())
        throw Exception::AlreadyBound{port};

    address_ = addr;
    port_ = port;

    bind();
}

void Server::bind(Address address)
{
    bind(address.num[0] << 24 | address.num[1] << 16 | address.num[2] << 8 | address.num[3], address.port);
}

std::unique_ptr<Socket> Server::accept() const
{
    if (isClose())
        throw Exception::NotBound{};

    const int sock = ::accept(sockServer_, NULL, NULL);
    if (sock < 0)
        throw Exception::ConnectionAccept{strerror(errno)};

    return std::unique_ptr<Socket>(new Socket(sock, address_, port_));
}

std::unique_ptr<Socket> Server::accept(int timeout) const
{
    if (isClose())
        throw Exception::NotBound{};

    std::vector<pollfd> fds(1);
    fds[0].fd = sockServer_;
    fds[0].events = POLLIN;

    const int status = ::poll(fds.data(), fds.size(), timeout);
    if (status < 0)
        throw Exception::Poll{strerror(errno)};
    if (status == 0)
        return nullptr;

    return accept();
}

void Server::close()
{
    if (sockServer_ >= 0)
        ::close(sockServer_);
    sockServer_ = -1;
}

bool Server::isClose() const
{
    return sockServer_ < 0;
}

}
