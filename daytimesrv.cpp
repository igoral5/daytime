/*
 * daytimesrv.cpp
 *
 *  Created on: 13.04.2012
 *      Author: igor
 */

#include <cstdlib>
#include <locale>
#include <exception>
#include <ctime>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <syslog.h>
#include <unistd.h>
#include <getopt.h>
#include "common.h"
#include "socket.h"
#include "reverse.h"
#include "sockaddrin.h"

const char *prog_name;

bool daemon_opt = false;

int
main(int argc, char *argv[]) try 
{
    std::locale::global(std::locale(""));
    prog_name = Util::prog(argv[0]);
    int arg;
    int port = PORT;
    bool daemon = false;
    while ((arg = getopt(argc, argv, "dp:")) != EOF)
    {
        switch (arg) {
        case 'd':
            daemon = true;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default:
            Util::out_message(false, std::cerr, 0, "Usage: %s [-d] [-p nom_port]", prog_name);
            exit(EXIT_FAILURE);
        }
    }
    if (daemon)
    {
        if (Util::makedaemon() < 0)
        {
            Util::out_message(false, std::cerr, 0, "%s: makedaemon: %s", prog_name, strerror(errno));
            exit(EXIT_FAILURE);
        }
        openlog(prog_name, LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT, LOG_LOCAL0);
        setlogmask(LOG_UPTO(LOG_DEBUG));
        syslog(LOG_LOCAL0|LOG_INFO, "Start %s", prog_name);
        daemon_opt = true;
    }
    Net::Socket listenfd(AF_INET, SOCK_STREAM, 0);
    int delay = 1;
    listenfd.SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &delay, sizeof(delay));
    Net::SockAddrIn servaddr;
    servaddr.get().sin_family = AF_INET;
    servaddr.get().sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.get().sin_port = htons(port);
    listenfd.Bind(servaddr, servaddr.length());
    listenfd.Listen(LISTENQ);
    for ( ; ; )
    {
        try
        {
            Net::SockAddrIn cliaddr;
            socklen_t cliaddr_len = cliaddr.length();
            Net::Socket connfd = listenfd.Accept(cliaddr, &cliaddr_len);
            try
            {
                Net::ReverseResolver reverse(cliaddr, cliaddr_len, 0);
                Util::out_message(daemon_opt, std::cout, LOG_LOCAL0|LOG_INFO, "Connect from %s:%s", reverse.HostName(), reverse.ServiceName());
            }
            catch(Net::ReverseResolverErr& err)
            {
                Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog_name, err.what());
            }
            time_t ticks = time(NULL);
            char buf[BUFSIZ];
            snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
            connfd.Write(buf, strlen(buf));
        }
        catch(Net::SocketErr& err)
        {
            Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog_name, err.what());
        }
    }
    return EXIT_SUCCESS;
}
catch(std::exception& err) 
{
    Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog_name, err.what());
    return EXIT_FAILURE;
}
catch(...) 
{
    Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog_name, "unknown exception");
    return EXIT_FAILURE;
}

