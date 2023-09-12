#pragma once

#include <memory>
#include "Socket.h"

namespace Socket
{

class Server
{
public:
    Server(unsigned int addr = 0, unsigned short port = 0);
    Server(Address address);
    virtual ~Server();
    Server(const Server& other) = delete;
    Server(Server&& other) noexcept;
    Server& operator=(const Server& other) = delete;
    Server& operator=(Server&& other) noexcept;

    void bind();
    void bind(unsigned int address, unsigned short port);
    void bind(Address address);
    std::unique_ptr<Socket> accept() const;
    std::unique_ptr<Socket> accept(int timeout) const;
    void close();
    bool isClose() const;

private:
    int sockServer_ = -1;
    unsigned int address_ = 0;
    unsigned short port_ = 0;
};

}
