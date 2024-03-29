/*
 * socket.cpp
 *
 *  Created on: 16.04.2012
 *      Author: igor
 */

#include "socket.h"
#include <cstring>
#include <errno.h>
#include <unistd.h>

Net::Socket::Socket(int domain, int type, int protocol)
{
    socket_ = socket(domain, type, protocol);
    if (socket_ < 0)
    {
        throw SocketErr(std::string("socket: ") + strerror(errno));
    }
}

Net::Socket::Socket(const Socket& sock)
{
    socket_ = sock.socket_;
}

Net::Socket::Socket(int sock)
{
    if (sock < 0)
    {
        throw SocketErr("not valid socket");
    }
    socket_ = sock;
}

Net::Socket& Net::Socket::operator=(const Socket& sock)
{
    if (this != &sock)
    {
       socket_=sock.socket_;
    }
    return *this;
}

Net::Socket::~Socket() throw()
{
    close(socket_);
}

void Net::Socket::Connect(const struct sockaddr *addr,socklen_t addrlen)
{
    if (connect(socket_, addr, addrlen) < 0)
    {
        throw SocketErr(std::string("connect: ") + strerror(errno));
    }
}

void Net::Socket::GetSockOpt(int level, int optname, void *optval, socklen_t *optlen)
{
    if (getsockopt(socket_, level, optname, optval, optlen) < 0)
    {
        throw SocketErr(std::string("getsockopt: ") + strerror(errno));
    }
}

void Net::Socket::SetSockOpt(int level, int optname, const void *optval, socklen_t optlen)
{
    if (setsockopt(socket_, level, optname, optval, optlen) < 0)
    {
        throw SocketErr(std::string("setsockopt: ") + strerror(errno));
    }
}

void Net::Socket::Bind(const struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(socket_, addr, addrlen) < 0)
    {
        throw SocketErr(std::string("bind: ") + strerror(errno));
    }
}

void Net::Socket::Listen(int backlog)
{
    if (listen(socket_, backlog) < 0)
    {
        throw SocketErr(std::string("listen: ") + strerror(errno));
    }
}

Net::Socket Net::Socket::Accept(struct sockaddr *addr, socklen_t *addrlen)
{
    int sock;
    for (;;)
    {
        sock = accept(socket_, addr, addrlen);
        if (sock < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            throw SocketErr(std::string("accept: ") + strerror(errno));
        }
        break;
    }
    return Socket(sock);
}

Net::Socket Net::Socket::Accept(struct sockaddr *addr, socklen_t *addrlen, int flags)
{
    int sock;
    for (;;)
    {
        sock = accept4(socket_, addr, addrlen, flags);
        if (sock < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            throw SocketErr(std::string("accept4: ") + strerror(errno));
        }
        break;
    }
    return Socket(sock);
}

ssize_t Net::Socket::Write(const void *buf, size_t count)
{
    ssize_t rc = write(socket_, buf, count);
    if (rc < 0)
    {
        throw SocketErr(std::string("write: ") + strerror(errno));
    }
    return rc;
}

ssize_t Net::Socket::Read(void *buf, size_t count)
{
    ssize_t rc = read(socket_, buf, count);
    if (rc < 0)
    {
        throw SocketErr(std::string("read: ") + strerror(errno));
    }
    return rc;
}

void Net::Socket::Close()
{
    if (close(socket_) < 0)
    {
        throw SocketErr(std::string("close: ") + strerror(errno));
    }
}




