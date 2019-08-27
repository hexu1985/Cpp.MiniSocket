/** \example tcpcliserv/tcpcli.cpp
 * This is an example of how to use the TCPSocket class to implement tcp echo client.
 */
#include <iostream>
#include <cstdlib>
#include "mini_socket.hpp"
#include "str_cli.hpp"

using namespace std;
using namespace mini_socket;

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

    str_cli(cin, sock);

    return 0;
}
