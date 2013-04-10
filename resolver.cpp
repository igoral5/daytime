/*
 * resolver.cpp
 *
 *  Created on: 16.04.2012
 *      Author: igor
 */
#include "resolver.h"
#include <cstring>
#include <errno.h>


Net::Resolver::Resolver(const char *node, const char *service, const AddrInfo& hints)
{
    int rc = getaddrinfo(node, service, &hints.get(), &res_);
    if (rc)
    {
        if (rc == EAI_SYSTEM)
        {
            throw ResolverErr(std::string("getaddrinfo: ") + strerror(errno));
        }
        else
        {
            throw ResolverErr(std::string("getaddrinfo: ") + gai_strerror(rc));
        }
    }
}

Net::Resolver::~Resolver() throw()
{
    freeaddrinfo(res_);
}

Net::Resolver::const_iterator Net::Resolver::begin() const
{
    return const_iterator(res_);
}

Net::Resolver::const_iterator Net::Resolver::end() const
{
    return const_iterator();
}

Net::Resolver::const_iterator::const_iterator() : node_(0)
{
}

Net::Resolver::const_iterator::const_iterator(const struct addrinfo *node) : node_(node)
{
}

Net::Resolver::const_iterator::const_iterator(const const_iterator& iter) : node_(iter.node_)
{
}

Net::Resolver::const_iterator& Net::Resolver::const_iterator::operator=(const const_iterator& iter)
{
    if (this != &iter)
        node_ = iter.node_;
    return *this;
}

bool Net::Resolver::const_iterator::operator!=(const const_iterator& iter) const
{
    return node_ != iter.node_;
}

bool Net::Resolver::const_iterator::operator==(const const_iterator& iter) const
{
    return node_ == iter.node_;
}

Net::Resolver::const_iterator& Net::Resolver::const_iterator::operator++()
{
    node_ = node_->ai_next;
    return *this;
}

const struct addrinfo& Net::Resolver::const_iterator::operator*() const
{
    return *node_;
}

const struct addrinfo* Net::Resolver::const_iterator::operator->() const
{
    return node_;
}



