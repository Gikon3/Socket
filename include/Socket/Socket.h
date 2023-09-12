#pragma once

#include <vector>
#include <string>

namespace Socket
{

struct Address
{
    unsigned char num[4];
    unsigned short port;
};

class Server;

class Socket
{
    friend Server;
public:
    Socket(unsigned int addr = 0, unsigned short port = 0);
    Socket(Address address);
    virtual ~Socket();
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    void connect();
    void connect(unsigned int addr, unsigned short port);
    void connect(Address address);
    int send(const std::vector<char>& data) const;
    int send(const std::string& str) const;
    std::vector<char> recvVec(int timeout = -1);
    std::string recvStr(int timeout = -1);
    void close();
    bool isClose() const;

private:
    Socket(int sock, unsigned int addr, unsigned short port);
    int sendRaw(const char* data, std::size_t size) const;
    bool poll(int timeout) const;

private:
    int sock_ = -1;
    unsigned int address_ = 0;
    unsigned short port_ = 0;
};

Socket& operator<<(Socket& socket, const std::vector<char>& data);
Socket& operator<<(Socket& socket, const std::string& str);
Socket& operator>>(Socket& socket, std::vector<char>& data);
Socket& operator>>(Socket& socket, std::string& str);

}
