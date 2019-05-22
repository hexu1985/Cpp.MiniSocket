/** \example daytimeudpcli.cpp
 * This is an example of how to use the UDPSocket class to implement daytimeudp client.
 */
#include <iostream>
#include <cstdlib>
#include "mini_socket.hpp"

using namespace std;
using namespace MiniSocket;

int
main(int argc, char **argv)
{
    if (argc != 3) {
        cout << "usage: a.out ip port" << endl;
        exit(-1);
    }

    UDPSocket sock;
    SocketAddress addr(argv[1], stoi(argv[2]));

    cout << "send to " << addr.toString() << endl;
    sock.sendTo("", 1, addr);

    SocketAddress   srcAddr;
    const int       MAXLINE = 1024;
    char            recvline[MAXLINE + 1];
    int             n;
	n = sock.recvFrom(recvline, MAXLINE, srcAddr);
    cout << "recv from " << srcAddr.toString() << endl;
	recvline[n] = '\0';	/* null terminate */
    cout << recvline << endl;

    return 0;
}
