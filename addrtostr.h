/*
 * addrtostr.h
 *
 *  Created on: 17.04.2012
 *      Author: igor
 */

#ifndef ADDRTOSTR_H_
#define ADDRTOSTR_H_

#include <exception>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <netdb.h>

#define PORT_LEN 6

namespace Net
{

class AddrToStrErr : public std::exception
{
public:
    explicit AddrToStrErr(const std::string& msg) : msg_(msg) {}
    virtual ~AddrToStrErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class AddrToStr
{
public:
    explicit AddrToStr(const struct addrinfo& addr, size_t buf_len = INET6_ADDRSTRLEN);
    virtual ~AddrToStr() throw();
    const char *Translate(const struct addrinfo& addr, size_t buf_len = INET6_ADDRSTRLEN);
    const char *get() const;
private:
    AddrToStr(const AddrToStr&); // without implementation
    AddrToStr& operator=(const AddrToStr&); // without implementation
    void Work(const struct addrinfo& addr, size_t buf_len);
    std::string str_;
};

} // namespace Net


#endif /* ADDRTOSTR_H_ */
