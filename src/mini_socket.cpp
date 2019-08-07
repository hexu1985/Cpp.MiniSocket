#include "mini_socket.hpp"
#include "SYSException.hpp"
#include "GAIException.hpp"

#include <sstream>
#include <cstring>

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

namespace MiniSocket {

// SocketStreamBuffer 
template <class CharT, class Traits = std::char_traits<CharT> >
class SocketStreamBuffer : public std::basic_streambuf<CharT, Traits> {
public:
    typedef typename Traits::int_type                 int_type;

    SocketStreamBuffer(TCPSocket *sock): sock_(sock) 
    {
        this->setg(inBuffer_, inBuffer_ + sizeof(inBuffer_),
                inBuffer_ + sizeof(inBuffer_));
        this->setp(outBuffer_, outBuffer_ + sizeof(outBuffer_));
    }

protected:
    int_type overflow(int_type c = Traits::eof()) 
    {
        this->sync();

        if (c != Traits::eof()) {
            this->sputc(Traits::to_char_type(c));
        }

        return 0;
    }

    int sync() 
    {
        sock_->sendAll(outBuffer_, (this->pptr() - outBuffer_) * sizeof(CharT));
        this->setp(outBuffer_, outBuffer_ + sizeof(outBuffer_));
        return 0;
    }

    int_type underflow() 
    {
        size_t len = sock_->recv(inBuffer_, sizeof(inBuffer_) * sizeof(CharT));

        if (len == 0) {
            return Traits::eof();
        }

        this->setg(inBuffer_, inBuffer_, inBuffer_ + len / sizeof(CharT));
        return this->sgetc();
    }

private:
    TCPSocket *sock_;

    CharT inBuffer_[1024];
    CharT outBuffer_[1024];
};

// TCPSocket 
TCPSocket::TCPSocket(SOCKET sockDesc)
{
    sockDesc_ = sockDesc;
}

TCPSocket::TCPSocket(const SocketAddress &foreignAddress)
{
    int domain = foreignAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_STREAM, 0);
    connect(foreignAddress);
}

void TCPSocket::sendAll(const char *buffer, int bufferLen)
{
	auto ptr = (const char *) buffer;
	auto nleft = bufferLen;
    int nwritten = 0;
	while (nleft > 0) {
		nwritten = send(ptr, nleft);
		nleft -= nwritten;
		ptr   += nwritten;
	}
}

std::iostream &TCPSocket::getStream()
{
    if (myStream_ == NULL) {
        myStreambuf_ = new SocketStreamBuffer<char>(this);
        myStream_ = new iostream(myStreambuf_);
    }
    return *myStream_;
}

// TCPServerSocket
TCPServerSocket::TCPServerSocket(const SocketAddress &localAddress)
{
    int domain = localAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_STREAM, 0);
    bind(localAddress);
	const int LISTENQ = 1024;
	listen(LISTENQ);
}

void TCPServerSocket::listen(int backlog)
{
    if (::listen(sockDesc_, backlog) != 0) {
        sys_error("listen error");
    }
}

shared_ptr<TCPSocket> TCPServerSocket::accept()
{
    SOCKET newConnSD;
    if ((newConnSD = ::accept(sockDesc_, NULL, 0)) == INVALID_SOCKET) {
        sys_error("Accept failed (accept())");
    }

    return shared_ptr<TCPSocket>(new TCPSocket(newConnSD));
}

// UDPSocket
UDPSocket::UDPSocket(const SocketAddress &localAddress)
{
    int domain = localAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_DGRAM, 0);
    bind(localAddress);
}

int UDPSocket::sendTo(const char *buffer, int bufferLen,
        const SocketAddress &foreignAddress)
{
    if (!isOpened()) {
        int domain = foreignAddress.getSockaddr()->sa_family;
        createSocket(domain, SOCK_DGRAM, 0);
    }

    int n = ::sendto(sockDesc_, buffer, bufferLen, 0,
            foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    if ( n < 0 ) {
        sys_error("Send failed (sendto())");
    }

    return n;
}

int UDPSocket::recvFrom(char *buffer, int bufferLen,
            SocketAddress &sourceAddress)
{
    sockaddr_storage cliAddr;
    socklen_t addrLen = sizeof(cliAddr);
    int n = recvfrom(sockDesc_, buffer, bufferLen, 0,
            (sockaddr *) &cliAddr, (socklen_t *) &addrLen);
    if (n < 0) {
        sys_error("Receive failed (recvfrom())");
    }
    sourceAddress = SocketAddress((sockaddr *)&cliAddr, addrLen);

    return n;
}

// UDPClientSocket
UDPClientSocket::UDPClientSocket(const SocketAddress &foreignAddress)
{
    int domain = foreignAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_STREAM, 0);
    connect(foreignAddress);
}

void UDPClientSocket::disconnect()
{
    sockaddr_in nullAddr;
    memset(&nullAddr, 0, sizeof(nullAddr));
    nullAddr.sin_family = AF_UNSPEC;

    if (::connect(sockDesc_, (sockaddr *) &nullAddr, sizeof (nullAddr)) != 0) {
#ifdef WIN32
        if (WSAGetLastError() != WSAEAFNOSUPPORT)
#else
        if (errno != EAFNOSUPPORT)
#endif
            sys_error("Disconnect failed (connect())");
    }
}

// DNSResolver 
DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, addrinfo *hints)
{
    addrinfo *res;
    int n;
    if ( (n = getaddrinfo(host, serv, hints, &res)) != 0) {
        gai_error("Resolve DNS query failed (getaddrinfo())", n);
    }

    auto deleter = [](addrinfo *ptr) {
#ifndef NDEBUG
                        cout << "freeaddrinfo called" << endl;
#endif
                        freeaddrinfo(ptr);
                    };

    return Iterator(std::shared_ptr<addrinfo>(res, deleter));
}

DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, 
        TransportLayerType trans_type)
{
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = (int) trans_type;

    return query(host, serv, &hints);
}

DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, 
        NetworkLayerType net_type, TransportLayerType trans_type)
{
    addrinfo hints = {};
    hints.ai_family = (int) net_type;
    hints.ai_socktype = (int) trans_type;

    return query(host, serv, &hints);
}

}   // namesapce MiniSocket
