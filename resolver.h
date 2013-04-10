/*
 * resolver.h
 *
 *  Created on: 16.04.2012
 *      Author: igor
 */

#ifndef RESOLVER_H_
#define RESOLVER_H_

#include <exception>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "addrinfo.h"

namespace Net
{

class ResolverErr : public std::exception
{
public:
    explicit ResolverErr(const std::string& msg) : msg_(msg) {}
    virtual ~ResolverErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class Resolver
{
public:
    Resolver(const char *node, const char *service, const AddrInfo& hints);
    virtual ~Resolver() throw ();
    class const_iterator
    {
    public:
        const_iterator();
        explicit const_iterator(const struct addrinfo *node);
        const_iterator(const const_iterator& iter);
        const_iterator& operator=(const const_iterator& iter);
        bool operator!=(const const_iterator& iter) const;
        bool operator==(const const_iterator& iter) const;
        const_iterator& operator++();
        const struct addrinfo& operator*() const;
        const struct addrinfo* operator->() const;
    private:
        const struct addrinfo *node_;
    };
    const_iterator begin() const;
    const_iterator end() const;
private:
    Resolver(const Resolver&); // without implementation
    Resolver& operator=(const Resolver&); // without implementation
    struct addrinfo *res_;
};

} // namespace Net

#endif /* RESOLVER_H_ */
