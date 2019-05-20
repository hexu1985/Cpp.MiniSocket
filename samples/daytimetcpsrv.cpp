#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mini_socket.hpp"

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
    TCPServerSocket server(addr);

    const int   MAXLINE = 1024; 
    char        buff[MAXLINE];
    time_t      ticks;
    for ( ; ; ) {
        auto sock = server.accept();
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        sock->sendAll(buff, strlen(buff));
    }

    return 0;
}
