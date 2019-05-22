/** \example daytimetcpcli.cpp
 * This is an example of how to use the TCPSocket class to implement daytimetcp client.
 */
#include <iostream>
#include <cstdlib>
#include "mini_socket.hpp"

using namespace std;
using namespace MiniSocket;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "usage: a.out ip port" << endl;
        exit(-1);
    }

    SocketAddress addr(argv[1], stoi(argv[2]));
    cout << "connect to " << addr.toString() << endl;
    TCPSocket sock(addr);
    cout << "connect to " << addr.toString() << " ok" << endl;

    const int MAXLINE = 1024;
    char      recvline[MAXLINE + 1];
    int       n;
    while ( (n = sock.recv(recvline, MAXLINE)) > 0) {
        recvline[n] = 0;    /* null terminate */
        cout << recvline << endl;
    }

    return 0;
}
