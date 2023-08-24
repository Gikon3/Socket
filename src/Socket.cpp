#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
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
    operator=(other);
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

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(addr);
    if (::connect(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw Exception::Connect{strerror(errno)};
}

template<typename T>
bool Socket::send(const T& data, int timeout)
{
    if (true)
        return sendRaw(data.data(), data.size());

    return sendRaw(data.c_str(), data.length());
}

std::vector<char> Socket::recvVec(int timeout)
{
    if (timeout < 0) {
    }
    else {
    }
}

std::string Socket::recvStr(int timeout)
{
    if (timeout < 0) {
    }
    else {
    }
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

bool Socket::sendRaw(const char* data, std::size_t size)
{
    if (timeout < 0) {
        return ::send(sock_, data, size, 0) == size;
    }
    else {
    }
}

Socket& operator<<(Socket& socket, const std::vector<char>& data)
{
    socket.send(data);
    return socket;
}

Socket& operator<<(Socket& socket, const std::string& str)
{
    socket.send(data);
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
