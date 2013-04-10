PROJECT( daytime CXX )
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.2 FATAL_ERROR )
ADD_EXECUTABLE( daytimecli  daytimecli.cpp common.h common.cpp socket.h socket.cpp resolver.h resolver.cpp addrinfo.h sockaddrin.h addrtostr.h addrtostr.cpp)
ADD_EXECUTABLE( daytimesrv  daytimesrv.cpp common.h common.cpp socket.h socket.cpp reverse.h reverse.cpp sockaddrin.h)
