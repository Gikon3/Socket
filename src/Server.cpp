#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include "Server.h"
#include "Exception.h"

namespace Socket
{

Server::Server()
{
}

Server::Server(int port)
{
    bind(port);
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

void Server::bind(int port)
{
    if (!isClose())
        throw Exception::AlreadyBound{port};

    if (port < 0)
        throw Exception::SmallPort{port};
    if (port > 65535)
        throw Exception::BigPort{port};

    sockServer_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockServer_ < 0)
        throw Exception::Create{strerror(errno)};

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(sockServer_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw Exception::Bind{strerror(errno)};

    if (::listen(sockServer_, 1) != 0)
        throw Exception::Listen{strerror(errno)};
}

std::unique_ptr<Socket> Server::accept() const
{
    if (isClose())
        throw Exception::NotBound{};

    const int sock = ::accept(sockServer_, NULL, NULL);
    if (sock < 0)
        throw Exception::ConnectionAccept{strerror(errno)};

    return std::unique_ptr<Socket>(new Socket(sock));
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
