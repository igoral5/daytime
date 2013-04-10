/*
 * daytimecli.cpp
 *
 *  Created on: 12.04.2012
 *      Author: igor
 */

#include <cstdlib>
#include <locale>
#include <exception>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include "common.h"
#include "socket.h"
#include "resolver.h"
#include "addrinfo.h"
#include "sockaddrin.h"
#include "addrtostr.h"

const char *prog_name;

int
main(int argc, char *argv[]) try 
{
    std::locale::global(std::locale(""));
    prog_name = Util::prog(argv[0]);
    if (argc != 3)
    {
        Util::out_message(false, std::cerr, 0, "Usage: %s hostname port", prog_name);
        exit(EXIT_FAILURE);
    }
    char *host_name = argv[1];
    char *port = argv[2];
    Net::AddrInfo hints;
    hints.get().ai_socktype = SOCK_STREAM;
    hints.get().ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
    Net::Resolver res(host_name, port, hints);
    Net::Resolver::const_iterator it = res.begin();
    Net::Resolver::const_iterator it_end = res.end();
    for (;it != it_end; ++it)
    {
        Net::PSockAddrIn servaddr(it->ai_addr);
        Net::AddrToStr trans(*it);
        Net::Socket sockfd(servaddr->sin_family, it->ai_socktype, it->ai_protocol);
        try
        {
            sockfd.Connect(servaddr, it->ai_addrlen);
        }
        catch (Net::SocketErr& err)
        {
            Util::out_message(false, std::cerr, 0, "%s: %s %s", prog_name, trans.get(), err.what());
            continue;
        }
        char recvline[BUFSIZ];
        ssize_t len;
        while ( (len = sockfd.Read(recvline, BUFSIZ)) > 0)
        {
            recvline[len] = '\0';    /* null terminate */
            std::cout << recvline;
        }
        break;
    }
    if (it == it_end)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
catch(std::exception& err) 
{
    Util::out_message(false, std::cerr, 0, "%s: %s", prog_name, err.what());
    exit(EXIT_FAILURE);
}
catch(...) 
{
    Util::out_message(false, std::cerr, 0, "%s: %s", prog_name, "unknown exception");
    exit(EXIT_FAILURE);
}


