#include "mini_socket.hpp"

#include <sstream>
#include <cstring>

#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

#if defined WIN32 or defined _WIN32

#ifndef WSVERS
#define WSVERS MAKEWORD(2, 0)
#endif

namespace MiniSocket {

class _WSAStartupHolder_ {
public:
    _WSAStartupHolder_()
    {
        if (WSAStartup(WSVERS, &wsadata) != 0)
            sys_error("WSAStartup error");
#ifndef NDEBUG
        cout << "WSAStartup ok" << endl;
#endif
    } 

    ~_WSAStartupHolder_()
    {
        WSACleanup();
#ifndef NDEBUG
        cout << "WSACleanup ok" << endl;
#endif
    }

private:
    WSADATA wsadata;
};

shared_ptr<_WSAStartupHolder_> makeSharedWSAStartupHolder()
{
    static shared_ptr<_WSAStartupHolder_> shared_holder(new _WSAStartupHolder_);
    return shared_holder;
}

_WSAStartupSharedHolder_::_WSAStartupSharedHolder_()
{
#ifndef NDEBUG
    cout << "_WSAStartupSharedHolder_::_WSAStartupSharedHolder_()" << endl;
#endif
    sharedHolder_ = makeSharedWSAStartupHolder();
}

_WSAStartupSharedHolder_::~_WSAStartupSharedHolder_()
{
#ifndef NDEBUG
    cout << "_WSAStartupSharedHolder_::~_WSAStartupSharedHolder_()" << endl;
#endif
}

}   // namespace MiniSocket

#endif

namespace MiniSocket {

// SocketAddressView
SocketAddressView::SocketAddressView(const sockaddr *addrVal, socklen_t addrLenVal) : addr_(addrVal), addrLen_(addrLenVal)
{
}

string SocketAddressView::toString() const
{
    return to_string(getSockaddr(), getSockaddrLen());
}

tuple<std::string, uint16_t> SocketAddressView::getAddressPort() const
{
    return get_address_port(getSockaddr(), getSockaddrLen());
}

const sockaddr *SocketAddressView::getSockaddr() const 
{
    return addr_;
}

socklen_t SocketAddressView::getSockaddrLen() const 
{
    return addrLen_;
}

NetworkLayerType SocketAddressView::getNetworkLayerType() const
{
    return get_network_layer_type(getSockaddr(), getSockaddrLen());
}

// Socket
Socket::~Socket()
{
    if (isOpened())
        close();
}

void Socket::open(NetworkLayerType version, TransportLayerType type)
{
    createSocket(static_cast<int>(version), static_cast<int>(type), 0);   
}

bool Socket::open(NetworkLayerType version, TransportLayerType type, ErrorCode &ec)
{
    return createSocket(static_cast<int>(version), static_cast<int>(type), 0, ec);
}

void Socket::close()
{
#if defined WIN32 or defined _WIN32
  closesocket(sockDesc_);
#else
  shutdown(sockDesc_, SHUT_RD);
  ::close(sockDesc_);
#endif
  sockDesc_ = INVALID_SOCKET;
}

bool Socket::isOpened() const
{
    return sockDesc_ != INVALID_SOCKET; 
}

SocketAddress Socket::getLocalAddress() const
{
    sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getsockname(sockDesc_, (sockaddr *) &addr, &addrLen) != 0) {
        sys_error("Fetch of local address failed (getsockname())");
    }

    return SocketAddress((sockaddr *)&addr, addrLen);
}

void Socket::createSocket(int domain, int type, int protocol)
{
    if (isOpened())
        close();

    sockDesc_ = socket(domain, type, protocol);
    if (!isOpened()) {
        sys_error("Can't create socket");
    }
}

bool Socket::createSocket(int domain, int type, int protocol, ErrorCode &ec)
{
    if (isOpened())
        close();

    sockDesc_ = socket(domain, type, protocol);
    if (!isOpened()) {
        ec = get_last_sys_error();
        return false;
    }
    return true;
}

void Socket::bind(const SocketAddress &localAddress)
{
    if (::bind(sockDesc_, localAddress.getSockaddr(), localAddress.getSockaddrLen()) != 0) {
        sys_error("bind error");
    }
}

// CommunicatingSocket 
void CommunicatingSocket::connect(const SocketAddress &foreignAddress)
{
    if (::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen()) != 0) {
        sys_error("connect error");
    }
}

void CommunicatingSocket::connect(const SocketAddressView &foreignAddress)
{
    if (::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen()) != 0) {
        sys_error("connect error");
    }
}

bool CommunicatingSocket::connect(const SocketAddress &foreignAddress, ErrorCode &ec)
{
    int ret = ::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    if (ret != 0) {
        ec = get_last_sys_error();
        return false;
    }
    return true;
}

bool CommunicatingSocket::connect(const SocketAddressView &foreignAddress, ErrorCode &ec)
{
    int ret = ::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    if (ret != 0) {
        ec = get_last_sys_error();
        return false;
    }
    return true;
}

int CommunicatingSocket::send(const char *buffer, int bufferLen)
{
    int n = ::send(sockDesc_, buffer, bufferLen, 0);
    if ( n < 0 ) {
        sys_error("Send failed (send())");
    }

    return n;
}

int CommunicatingSocket::recv(char *buffer, int bufferLen)
{
    int n = ::recv(sockDesc_, buffer, bufferLen, 0); 
    if ( n < 0 ) {
        sys_error("Receive failed (recv())");
    }

    return n;
}

SocketAddress CommunicatingSocket::getForeignAddress() const
{
    sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getpeername(sockDesc_, (sockaddr *) &addr, &addrLen) != 0) {
        sys_error("Fetch of foreign address failed (getpeername())");
    }

    return SocketAddress((sockaddr *)&addr, addrLen);
}

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
