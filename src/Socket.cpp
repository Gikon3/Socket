#include "linuxApi.h"
#include "windowsApi.h"
#include <sstream>
#include "Socket.h"
#include "Exception.h"

namespace Socket
{

Socket::Socket(unsigned int addr, unsigned short port) :
    address_(addr), port_(port)
{
#if defined(WIN32)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
        throw Exception::WsaInit(errorStr());
#endif
}

Socket::Socket(Address address) :
    Socket(address.num[0] << 24 | address.num[1] << 16 | address.num[2] << 8 | address.num[3], address.port)
{
}

Socket::~Socket()
{
    close();
}

Socket::Socket(Socket&& other) noexcept
{
    operator=(std::move(other));
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    std::swap(sock_, other.sock_);
    return *this;
}

void Socket::connect()
{
    if (!isClose())
        throw Exception::AlreadyConnect{};

    sock_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0)
        throw Exception::Create{errorStr()};

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port_);
    saddr.sin_addr.s_addr = htonl(address_);
    if (::connect(sock_, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) != 0)
        throw Exception::Connect{errorStr()};
}

void Socket::connect(unsigned int addr, unsigned short port)
{
    if (!isClose())
        throw Exception::AlreadyConnect{};

    address_ = addr;
    port_ = port;

    connect();
}

void Socket::connect(Address address)
{
    connect(address.num[0] << 24 | address.num[1] << 16 | address.num[2] << 8 | address.num[3], address.port);
}

int Socket::send(const std::vector<char>& data) const
{
    return sendRaw(data.data(), data.size());
}

int Socket::send(const std::string& str) const
{
    return sendRaw(str.c_str(), str.length());
}

std::vector<char> Socket::recvVec(int timeout)
{
    if (isClose())
        throw Exception::NotConnect{};

    if (!poll(timeout))
        return std::vector<char>{};

    std::vector<char> accum;
    char buf[256];
    int len;
    do {
        len = ::recv(sock_, buf, sizeof(buf), 0);
        if (len < 0)
            throw Exception::Recv{errorStr()};
        accum.insert(accum.end(), buf, buf + len);
    } while (len > 0 && poll(0));

    return accum;
}

std::string Socket::recvStr(int timeout)
{
    if (isClose())
        throw Exception::NotConnect{};

    if (!poll(timeout))
        return std::string{};

    std::stringstream accum;
    char buf[256];
    const int bufLen = sizeof(buf) - 1;
    int len;
    do {
        len = ::recv(sock_, buf, sizeof(buf) - 1, 0);
        if (len < 0)
            throw Exception::Recv{errorStr()};
        buf[len] = '\0';
        accum << buf;
    } while (len > 0 && poll(0));

    return accum.str();
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

Socket::Socket(int sock, unsigned int addr, unsigned short port) :
    sock_(sock), address_(addr), port_(port)
{
}

int Socket::sendRaw(const char* data, std::size_t size) const
{
    if (isClose())
        throw Exception::NotConnect{};

    const int tsize = ::send(sock_, data, size, 0);
    if (tsize < 0)
        throw Exception::Send{errorStr()};

    return tsize;
}

bool Socket::poll(int timeout) const
{
    std::vector<pollfd> fds(1);
    fds[0].fd = sock_;
    fds[0].events = POLLIN;

    const int status = ::poll(fds.data(), fds.size(), timeout);
    if (status < 0)
        throw Exception::Poll{errorStr()};

    return status > 0;
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
