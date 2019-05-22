/** \example udpcli.cpp
 * This is an example of how to use the UDPSocket class to implement udp echo client.
 */
#include <iostream>
#include <cstdlib>
#include "mini_socket.hpp"
#include "dg_cli.hpp"

using namespace std;
using namespace MiniSocket;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "usage: a.out ip port" << endl;
        exit(-1);
    }

    SocketAddress addr(argv[1], stoi(argv[2]));
    UDPSocket sock;

	dg_cli(cin, sock, addr);

    return 0;
}
