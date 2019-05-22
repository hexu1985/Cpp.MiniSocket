/** \example str_echo.cpp
 * The implement of str_echo function in tcp echo server.
 */
#include "str_echo.hpp"

using namespace std;
using namespace MiniSocket;

void
str_echo(TCPSocket &sock)
{
    const int   MAXLINE = 4096;
    char        buf[MAXLINE];

    int n = 0;
    while ( (n = sock.recv(buf, MAXLINE)) > 0)
        sock.sendAll(buf, n);
}
