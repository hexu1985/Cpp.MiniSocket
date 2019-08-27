/** \example udpcliserv/dg_cli.cpp
 * The implement of dg_cli function in udp echo client.
 */
#include "dg_cli.hpp"

#include <string>

using namespace std;
using namespace mini_socket;

void
dg_cli(istream &in, UDPSocket &sock, SocketAddress &addr)
{
	int	            n;
    const int       MAXLINE = 4096;
	char	        recvline[MAXLINE + 1];
    SocketAddress   srcAddr;
    string          sendline;
    while (getline(in, sendline)) {
        sock.sendTo(sendline.c_str(), sendline.size(), addr);
        n = sock.recvFrom(recvline, MAXLINE, srcAddr);
        recvline[n] = '\0';
        cout << recvline << endl;
    }
}

