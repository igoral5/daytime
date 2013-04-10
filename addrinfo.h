/*
 * addrinfo.h
 *
 *  Created on: 17.04.2012
 *      Author: igor
 */

#ifndef ADDRINFO_H_
#define ADDRINFO_H_

#include <cstring>
#include <netdb.h>

namespace Net
{

class AddrInfo
{
public:
    AddrInfo()
    {
        bzero(&addr_, sizeof(addr_));
    }
    struct addrinfo& get()
    {
        return addr_;
    }
    const struct addrinfo& get() const
    {
        return addr_;
    }
private:
    struct addrinfo addr_;
};

} // namespace Net


#endif /* ADDRINFO_H_ */
