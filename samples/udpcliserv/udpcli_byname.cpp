/** \example udpcli_byname.cpp
 * This is an example of how to use the udp_connect to implement udp echo client.
 */
#include <iostream>
#include <cstdlib>
#include "mini_socket.hpp"
#include "udp_connect.hpp"
#include "dg_cli_connected.hpp"

using namespace std;
using namespace MiniSocket;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "usage: a.out ip port" << endl;
        exit(-1);
    }

    auto sock = udp_connect(argv[1], argv[2]);

	dg_cli(cin, *sock);

    return 0;
}
