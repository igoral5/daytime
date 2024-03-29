/*
 * socket.h
 *
 *  Created on: 16.04.2012
 *      Author: igor
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <exception>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

namespace Net
{

class SocketErr : public std::exception
{
public:
    explicit SocketErr(const std::string& msg) : msg_(msg) {}
    virtual ~SocketErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class Socket
{
public:
    Socket(int domain, int type, int protocol);
    Socket(const Socket& sock);
    explicit Socket(int sock);
    Socket& operator=(const Socket& sock);
    virtual ~Socket() throw();
    void Connect(const struct sockaddr *addr,socklen_t addrlen);
    void GetSockOpt(int level, int optname, void *optval, socklen_t *optlen);
    void SetSockOpt(int level, int optname, const void *optval, socklen_t optlen);
    void Bind(const struct sockaddr *addr, socklen_t addrlen);
    void Listen(int backlog);
    Socket Accept(struct sockaddr *addr, socklen_t *addrlen);
    Socket Accept(struct sockaddr *addr, socklen_t *addrlen, int flags);
    ssize_t Write(const void *buf, size_t count);
    ssize_t Read(void *buf, size_t count);
    void Close();
private:
    int socket_;
};

} // namespace Net

#endif /* SOCKET_H_ */
