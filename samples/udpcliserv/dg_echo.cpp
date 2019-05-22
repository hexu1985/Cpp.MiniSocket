/** \example dg_echo.cpp
 * The implement of dg_echo function in udp echo server.
 */
#include "dg_echo.hpp"
#include <iostream>

using namespace std;
using namespace MiniSocket;

void
dg_echo(UDPSocket &sock)
{
	int			    n;
    const int       MAXLINE = 4096;
	char		    mesg[MAXLINE];
    SocketAddress cliaddr;

	for ( ; ; ) {
		n = sock.recvFrom(mesg, MAXLINE, cliaddr);
        cout << "recvfrom " << cliaddr.toString() << endl;
        sock.sendTo(mesg, n, cliaddr);
	}
}
