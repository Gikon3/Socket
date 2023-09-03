#pragma once

#include <stdexcept>

namespace Socket
{
namespace Exception
{

class Exception : public std::runtime_error
{
public:
    Exception(const std::string& str) : std::runtime_error("Socket exception. " + str) {}
};

class SmallPort : public Exception
{
public:
    SmallPort(int port) : Exception("Port " + std::to_string(port) + " is too small") {}
};

class BigPort : public Exception
{
public:
    BigPort(int port) : Exception("Port " + std::to_string(port) + " is too big") {}
};

class AlreadyBound : public Exception
{
public:
    AlreadyBound(int port) : Exception("Socket is already bound: " + std::to_string(port)) {}
};

class Create : public Exception
{
public:
    Create(const std::string& str) : Exception("Create fail: " + str) {}
};

class Bind : public Exception
{
public:
    Bind(const std::string& str) : Exception("Bind fail: " + str) {}
};

class Listen : public Exception
{
public:
    Listen(const std::string& str) : Exception("Listen fail: " + str) {}
};

class NotBound : public Exception
{
public:
    NotBound() : Exception("Socket is not bound") {}
};

class ConnectionAccept : public Exception
{
public:
    ConnectionAccept(const std::string& str) : Exception("Connection accept fail: " + str) {}
};

class Poll : public Exception
{
public:
    Poll(const std::string& str) : Exception("Poll fail: " + str) {}
};

class Connect : public Exception
{
public:
    Connect(const std::string& str) : Exception("Connect fail: " + str) {}
};

class NotConnect : public Exception
{
public:
    NotConnect() : Exception("Socket is not connect") {}
};

class Recv : public Exception
{
public:
    Recv(const std::string& str) : Exception("Receive fail: " + str) {}
};

}
}
