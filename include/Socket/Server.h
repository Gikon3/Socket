#pragma once

#include <memory>
#include "Socket.h"

namespace Socket
{

class Server
{
public:
    Server();
    Server(int port);
    virtual ~Server();
    Server(const Server& other) = delete;
    Server(Server&& other);
    Server& operator=(const Server& other) = delete;
    Server& operator=(Server&& other);

    void bind(int port);
    std::unique_ptr<Socket> accept() const;
    std::unique_ptr<Socket> accept(int timeout) const;
    void close();
    bool isClose() const;

private:
    int sockServer_ = -1;
};

}
