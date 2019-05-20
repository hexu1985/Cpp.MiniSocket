#include "str_cli.hpp"

#include <string>

using namespace std;
using namespace MiniSocket;

void
str_cli(istream &in, TCPSocket &sock)
{
    string sendline, recvline;
    auto &sockStream = sock.getStream();

    while (getline(in, sendline)) {
        sendline += '\n';
        sock.sendAll(sendline.c_str(), sendline.size());
        getline(sockStream, recvline);
        cout << recvline << endl;
    }
}
