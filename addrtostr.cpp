/*
 * addrtostr.cpp
 *
 *  Created on: 18.04.2012
 *      Author: igor
 */
#include "addrtostr.h"
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <cstdio>



Net::AddrToStr::AddrToStr(const struct addrinfo& addr, size_t buf_len)
{
    Work(addr, buf_len);
}

Net::AddrToStr::~AddrToStr() throw()
{
}

const char *Net::AddrToStr::Translate(const struct addrinfo& addr, size_t buf_len)
{
    Work(addr, buf_len);
    return str_.c_str();
}

const char *Net::AddrToStr::get() const
{
    return str_.c_str();
}

void Net::AddrToStr::Work(const struct addrinfo& addr, size_t buf_len)
{
    if (addr.ai_canonname)
    {
        str_ = addr.ai_canonname;
    }
    else
    {
        char buf[buf_len];
        if (!inet_ntop(addr.ai_family, &reinterpret_cast<struct sockaddr_in *>(addr.ai_addr)->sin_addr, buf, addr.ai_addrlen))
        {
            throw AddrToStrErr(std::string("inet_ntop: ") + strerror(errno));
        }
        str_ =  buf;
    }
    str_ += ':';
    char port[PORT_LEN];
    snprintf(port, PORT_LEN, "%d", ntohs(reinterpret_cast<struct sockaddr_in *>(addr.ai_addr)->sin_port));
    str_ += port;
}




