#pragma once

#include <memory>
#include "Socket.h"

namespace Socket
{

class Server
{
public:
    Server();
    Server(unsigned int addr, unsigned short port);
    Server(Address address);
    virtual ~Server();
    Server(const Server& other) = delete;
    Server(Server&& other);
    Server& operator=(const Server& other) = delete;
    Server& operator=(Server&& other);

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
