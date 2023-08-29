#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <sstream>
#include "Socket.h"
#include "Exception.h"

namespace Socket
{

Socket::Socket()
{
}

Socket::Socket(unsigned long addr, int port)
{
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0)
        throw Exception::Create{strerror(errno)};

    connect(addr, port);
}

Socket::~Socket()
{
    close();
}

Socket::Socket(Socket&& other)
{
    operator=(std::move(other));
}

Socket& Socket::operator=(Socket&& other)
{
    std::swap(sock_, other.sock_);
    return *this;
}

void Socket::connect(unsigned long addr, int port)
{
    if (port < 0)
        throw Exception::SmallPort{port};
    else if (port > 65535)
        throw Exception::BigPort{port};

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(addr);
    if (::connect(sock_, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) < 0)
        throw Exception::Connect{strerror(errno)};
}

std::vector<char> Socket::recvVec(int size, int timeout)
{
    if (sock_ < 0)
        throw Exception::NotConnect{};

    if (timeout >= 0 && !poll(timeout))
        return std::vector<char>{};

    if (size < 0)
        return recvVecInt(timeout);

    return recvVecInt(size, timeout);
}

std::string Socket::recvStr(int size, int timeout)
{
    if (sock_ < 0)
        throw Exception::NotConnect{};

    if (timeout >= 0 && !poll(timeout))
        return std::string{};

    if (size < 0)
        return recvStrInt(timeout);

    return recvStrInt(size, timeout);
}

void Socket::close()
{
    if (sock_ >= 0)
        ::close(sock_);
    sock_ = -1;
}

bool Socket::isClose() const
{
    return sock_ < 0;
}

Socket::Socket(int sock) :
    sock_(sock)
{
}

bool Socket::sendRaw(const char* data, std::size_t size, int timeout)
{
    // TODO add timeout
    return ::send(sock_, data, size, 0) == size;
}

bool Socket::poll(int timeout) const
{
    std::vector<pollfd> fds(1);
    fds[0].fd = sock_;
    fds[0].events = POLLIN;

    const int status = ::poll(fds.data(), fds.size(), timeout);
    if (status == -1)
        throw Exception::Poll{strerror(errno)};

    return status > 0;
}

std::vector<char> Socket::recvVecInt(int size, int timeout)
{
    std::vector<char> accum;
    char buf[256];
    int len;
    do {
        len = ::recv(sock_, buf, size >= 0 && size < sizeof(buf) ? size : sizeof(buf), 0);
        if (len < 0)
            throw Exception::Recv{strerror(errno)};
        accum.insert(accum.end(), buf, buf + len);
        if (size > 0)
            size -= len;
    }
    while (len > 0 && size > 0);

    return accum;
}

std::string Socket::recvStrInt(int size, int timeout)
{
    std::stringstream accum;
    char buf[256];
    const int bufLen = sizeof(buf) - 1;
    int len;
    do {
        len = ::recv(sock_, buf, size >= 0 && size < bufLen ? size : bufLen, 0);
        if (len < 0)
            throw Exception::Recv{strerror(errno)};
        buf[len] = '\0';
        accum << buf;
        if (size > 0)
            size -= len;
    }
    while (len > 0 && size > 0);

    return accum.str();
}

std::vector<char> Socket::recvVecInt(int timeout)
{
    std::vector<char> accum;
    char buf[256];
    int len;
    do {
        len = ::recv(sock_, buf, sizeof(buf), 0);
        if (len < 0)
            throw Exception::Recv{strerror(errno)};
        accum.insert(accum.end(), buf, buf + len);
    }
    while (len > 0 && poll(0));

    return accum;
}

std::string Socket::recvStrInt(int timeout)
{
    std::stringstream accum;
    char buf[256];
    int len;
    do {
        len = ::recv(sock_, buf, sizeof(buf) - 1, 0);
        if (len < 0)
            throw Exception::Recv{strerror(errno)};
        buf[len] = '\0';
        accum << buf;
    }
    while (len > 0 && poll(0));

    return accum.str();
}

Socket& operator<<(Socket& socket, const std::vector<char>& data)
{
    socket.send(data);
    return socket;
}

Socket& operator<<(Socket& socket, const std::string& str)
{
    socket.send(str);
    return socket;
}

Socket& operator>>(Socket& socket, std::vector<char>& data)
{
    data = socket.recvVec();
    return socket;
}

Socket& operator>>(Socket& socket, std::string& str)
{
    str = socket.recvStr();
    return socket;
}

}
