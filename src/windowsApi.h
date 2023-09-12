#pragma once

#if defined(WIN32)

#include <string>
#include <winsock2.h>

static inline std::string errorStr()
{
    static char buf[256];
    buf[0] = '\0';

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(),
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), buf, sizeof(buf), NULL);

    return std::string{buf};
}

static inline int poll(pollfd* fds, unsigned long size, int timeout)
{
    return ::WSAPoll(fds, size, timeout);
}

static inline int close(int sock)
{
    return ::closesocket(sock);
}
#endif