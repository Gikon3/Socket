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
    template<typename T>
    static constexpr bool isGoodType = std::same_as<T, std::string> || std::same_as<T, std::vector<char>>;
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
    bool send(const T& data, int timeout = -1) requires isGoodType<T>
    {
        if constexpr (std::same_as<T, std::vector<char>>)
            return sendRaw(data.data(), data.size(), timeout);
        else if constexpr (std::same_as<T, std::string>)
            return sendRaw(data.c_str(), data.length(), timeout);
        else
            return false;
    }
    std::vector<char> recvVec(int size = -1, int timeout = -1);
    std::string recvStr(int size = -1, int timeout = -1);
    void close();
    bool isClose() const;

private:
    Socket(int sock);
    bool sendRaw(const char* data, std::size_t size, int timeout);
    bool poll(int timeout) const;
    std::vector<char> recvVecInt(int size, int timeout);
    std::string recvStrInt(int size, int timeout);
    std::vector<char> recvVecInt(int timeout);
    std::string recvStrInt(int timeout);

private:
    int sock_ = -1;
};

Socket& operator<<(Socket& socket, const std::vector<char>& data);
Socket& operator<<(Socket& socket, const std::string& str);
Socket& operator>>(Socket& socket, std::vector<char>& data);
Socket& operator>>(Socket& socket, std::string& str);

}
