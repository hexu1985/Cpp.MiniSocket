/** \example tcpcliserv/tcpserv.cpp
 * This is an example of how to use the TCPServerSocket class to implement tcp echo server.
 */
#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <thread>
#include "mini_socket.hpp"
#include "str_echo.hpp"

using namespace std;
using namespace mini_socket;

static void doit(shared_ptr<TCPSocket> sock);

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
    TCPServerSocket server(addr);

    for ( ; ; ) {
        auto sock = server.accept();
        thread(doit, sock).detach();
    }

    return 0;
}

static void 
doit(shared_ptr<TCPSocket> sock)
{
    try {
        str_echo(*sock);
    } catch (const runtime_error &e) {
        cout << "str_echo error, " << e.what() << endl;
    }
}
