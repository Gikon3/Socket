#pragma once

#include <vector>
#include <string>
#include <concepts>

namespace Socket
{

class Server;

class Socket
{
    friend Server;
public:
    Socket();
    Socket(unsigned long addr, int port);
    virtual ~Socket();
    Socket(const Socket& other) = delete;
    Socket(Socket&& other);
    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other);

    void connect(unsigned long addr, int port);
    template<typename T>
    bool send(const T& data, int timeout = -1);
    std::vector<char> recvVec(int timeout = -1);
    std::string recvStr(int timeout = -1);
    void close();
    bool isClose() const;

private:
    Socket(int sock);
    bool sendRaw(const char* data, std::size_t size);

private:
    int sock_ = -1;
};

Socket& operator<<(Socket& socket, const std::vector<char>& data);
Socket& operator<<(Socket& socket, const std::string& str);
Socket& operator>>(Socket& socket, std::vector<char>& data);
Socket& operator>>(Socket& socket, std::string& str);

}
