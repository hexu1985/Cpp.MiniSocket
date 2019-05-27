/** \example dg_cli_connected.cpp
 * The implement of dg_cli function in udp echo client (using UDPClientSocket).
 */
#include "dg_cli_connected.hpp"

#include <string>

using namespace std;
using namespace MiniSocket;

void
dg_cli(istream &in, UDPClientSocket &sock)
{
	int	            n;
    const int       MAXLINE = 4096;
	char	        recvline[MAXLINE + 1];
    SocketAddress   srcAddr;
    string          sendline;
    while (getline(in, sendline)) {
        sock.send(sendline.c_str(), sendline.size());
        n = sock.recv(recvline, MAXLINE);
        recvline[n] = '\0';
        cout << recvline << endl;
    }
}

