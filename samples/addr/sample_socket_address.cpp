/** \example addr/sample_socket_address.cpp
 * This is an example of how to use the SocketAddress class.
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
    cout << addr.toString() << endl;

    return 0;
}
