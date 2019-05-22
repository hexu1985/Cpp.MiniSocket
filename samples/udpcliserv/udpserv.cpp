#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <thread>
#include "mini_socket.hpp"
#include "dg_echo.hpp"

using namespace std;
using namespace MiniSocket;

int main(int argc, char *argv[])
{
    unsigned short port = 9870;
    string ip = "0.0.0.0";

    if (argc == 2) {
        port = stoi(argv[1]);
    } else if (argc == 3) {
        ip = argv[1];
        port = stoi(argv[2]);
    } else {
        cout << "usage: a.out [ <ip> ] <port>" << endl;
        exit(-1);
    }

    SocketAddress addr(ip.c_str(), port);
    cout << "bind " << addr.toString() << endl;
    UDPSocket server(addr);

	dg_echo(server);
}
