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
    socketBind(port);
}

Server::~Server()
{
    close();
}

void Server::bind(int port)
{
    socketBind(port);
}

Socket Server::accept()
{
    if (sockServer_ < 0)
        throw Exception::NotBound{};

    const int sock = ::accept(sockServer_, NULL, NULL);
    if (sock < 0)
        throw Exception::ConnectionAcceptFail{strerror(errno)};

    return sock;
}

Socket Server::accept(int timeout)
{
    if (sockServer_ < 0)
        throw Exception::NotBound{};

    std::vector<pollfd> fds(1);
    fds[0].fd = sockServer_;
    fds[0].events = POLLIN;

    const int status = ::poll(fds.data(), fds.size(), timeout);
    if (status == -1)
        throw Exception::Poll{strerror(errno)};
    else if (status == 0)
        throw Exception::ConnectionAcceptFail{strerror(errno)};

    return accept();
}

void Server::close()
{
    if (sockServer_ >= 0)
        ::close(sockServer_);
    sockServer_ = -1;
}

bool Server::isBound() const
{
    return isBound_;
}

bool Server::isClose() const
{
    return sockServer_ >= 0;
}

void Server::socketBind(int port)
{
    if (port < 0)
        throw Exception::SmallPort{port};
    else if (port > 65535)
        throw Exception::BigPort{port};

    if (isBound_)
        throw Exception::AlreadyBound{port};

    sockServer_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockServer_ < 0)
        throw Exception::Create{strerror(errno)};

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(sockServer_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw Exception::Bind{strerror(errno)};

    if (listen(sockServer_, 1) != 0)
        throw Exception::Listen{strerror(errno)};

    isBound_ = true;
}

}
