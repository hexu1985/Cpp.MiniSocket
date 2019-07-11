/** \example tcpcliserv/tcpcli_byname.cpp
 * This is an example of how to use the tcp_connect to implement tcp echo client.
 */
#include <iostream>
#include <cstdlib>
#include "mini_socket.hpp"
#include "tcp_connect.hpp"
#include "str_cli.hpp"

using namespace std;
using namespace MiniSocket;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "usage: a.out <hostname/IPaddress> <service/port#>" << endl;
        exit(-1);
    }

	auto sock = tcp_connect(argv[1], argv[2]);

    str_cli(cin, *sock);

    return 0;
}
