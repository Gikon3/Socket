#pragma once

#if defined(UNIX)

#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

static inline const char* errorStr()
{
    return ::strerror(errno);
}
#endif