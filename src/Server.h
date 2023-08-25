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
    std::unique_ptr<Socket> accept();
    std::unique_ptr<Socket> accept(int timeout);
    void close();
    bool isBound() const;
    bool isClose() const;

private:
    void socketBind(int port);

private:
    int sockServer_ = -1;
    bool isBound_ = false;
};

}
