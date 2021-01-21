#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

#include "config.hpp"
#include "err_quit.hpp"
#include "tcp_connect.hpp"
#include "send_all.hpp"
#include "recv_until.hpp"

using namespace std;

void get_http_header(int sockfd, int *content_length)
{
    char   recvline[MAXLINE];
    string line;
    size_t pos;
    string key;
    while (true) {
        int n = recv_until(sockfd, recvline, MAXLINE, '\n');
        if (n < 0)
            err_quit("recv_until error");
        if (n == 0)
            err_quit("str_cli: server terminated prematurely");
        line.assign(recvline, n);
        cout << line << endl;

        if (line == "\r\n")
            break;

        pos = line.find(':');
        if (pos == string::npos) {
            continue;
        } 
        key = line.substr(0, pos);
        if (key == "Content-Length") {
            *content_length = stoi(line.substr(pos+1));
        }
    }
}

string get_http_body(int sockfd, int content_length) 
{
    string buffer;
    buffer.resize(content_length);
    int n = recv_until(sockfd, const_cast<char *>(buffer.data()), content_length);
    if (n < 0) 
        err_quit("recv_until error");
    else if (n == 0)
        err_quit("recv_until no data");
    return buffer;
}

void http_cli(int sockfd, const char *host)
{
    ostringstream http_req_os;
    http_req_os << "GET / HTTP/1.1\r\n"
        << "Host: " << host << "\r\n"
        << "Accept: */*\r\n\r\n";
    string http_req = http_req_os.str();
    if (send_all(sockfd, http_req.c_str(), http_req.length()) < 0)
        err_quit("send_all error");

    int content_length = 0;
    get_http_header(sockfd, &content_length);

    string http_body = get_http_body(sockfd, content_length);
    cout << http_body << endl; 
}

int main(int argc, char **argv)
{
    int sockfd;

    if (argc != 3)
        err_quit("usage: a.out <hostname> <port#>");

    sockfd = tcp_connect(argv[1], argv[2]);
    if (sockfd < 0)
        err_quit("tcp_connect error");

    http_cli(sockfd, argv[1]);

    exit(0);
}
