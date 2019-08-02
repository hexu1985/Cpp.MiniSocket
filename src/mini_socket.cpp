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
            throw_socket_exception("WSAStartup error");
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

string get_last_system_error_str()
{
    HLOCAL hlocal = NULL;
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
        WSAGetLastError(), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPTSTR) &hlocal, 0, NULL);
    std::string err_msg((const char *)hlocal);
    LocalFree(hlocal);

    return err_msg;
}

string get_gai_error_str(int n)
{
    return get_last_system_error_str();
}

}   // namespace MiniSocket

#else   // not win32

namespace MiniSocket {

string get_last_system_error_str()
{
    return strerror(errno);
}

string get_gai_error_str(int n)
{
    return gai_strerror(n);
}

}   // namespace MiniSocket

#endif

namespace { // anonymous namespace for details

tuple<string, uint16_t> get_address_port(const sockaddr *sa, socklen_t salen)
{
	static const tuple<string, uint16_t> null_result;
	char str[128] = {0};

	switch (sa->sa_family) {
	case AF_INET: {
		sockaddr_in	*sin = (sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return null_result;

        return make_tuple(std::string(str), ntohs(sin->sin_port));
	}

	case AF_INET6: {
		sockaddr_in6	*sin6 = (sockaddr_in6 *) sa;

		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
			return null_result;

        return make_tuple(std::string(str), ntohs(sin6->sin6_port));
	}

	default:
        return null_result;
	}
    return null_result;
}

string to_string(const sockaddr *sa, socklen_t salen)
{
    static const std::string null_result;

    string address;
    uint16_t port;
    std::tie(address, port) = get_address_port(sa, salen);
    if (address.empty())
        return null_result;

    ostringstream os;
    if (sa->sa_family == AF_INET6)
        os << "[";
    os << address;
    if (sa->sa_family == AF_INET6)
        os << "]";
    os << ":" << port;
    return os.str();
}

MiniSocket::NetworkLayerType get_network_layer_type(const sockaddr *sa, socklen_t salen)
{
    MiniSocket::NetworkLayerType type = MiniSocket::NetworkLayerType::UNKNOWN; 
	switch (sa->sa_family) {
	case AF_INET:
        type = MiniSocket::NetworkLayerType::IPv4;
        break;
	case AF_INET6:
        type = MiniSocket::NetworkLayerType::IPv6;
        break;
	default:
        break;
	}

    return type;
}

}   // namespace

namespace MiniSocket {

// SocketAddressView
SocketAddressView::SocketAddressView(const sockaddr *addrVal, socklen_t addrLenVal) : addr_(addrVal), addrLen_(addrLenVal)
{
}

string SocketAddressView::toString() const
{
    return ::to_string(getSockaddr(), getSockaddrLen());
}

tuple<std::string, uint16_t> SocketAddressView::getAddressPort() const
{
    return ::get_address_port(getSockaddr(), getSockaddrLen());
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
    return ::get_network_layer_type(getSockaddr(), getSockaddrLen());
}

// SocketAddress
SocketAddress::SocketAddress(const char *address, uint16_t port): SocketAddress()
{
    if (setAddressPort(address, port))
        return;

    ostringstream os;
    os << "Construct SocketAddress error: the address is [" << address << "], and port is [" << port << "]";
    throw_socket_exception(os.str());
}

SocketAddress::SocketAddress(sockaddr *addrVal, socklen_t addrLenVal):
    addrLen_(addrLenVal) 
{
    memcpy( &addr_, addrVal, addrLenVal );
}

bool SocketAddress::setAddressPort(const char *address, uint16_t port)
{
    sockaddr *sa = (sockaddr *) &addr_;

    sa->sa_family = AF_INET;
    sockaddr_in	*sin = (sockaddr_in *) sa;
    if (inet_pton(sa->sa_family, address, &sin->sin_addr) == 1) { // success
        sin->sin_port = htons(port);
        return true;
    }

    sa->sa_family = AF_INET6;
    sockaddr_in6 *sin6 = (sockaddr_in6 *) sa;
    if (inet_pton(sa->sa_family, address, &sin6->sin6_addr) == 1) { // success
        sin6->sin6_port = htons(port);
        return true;
    }

    return false;
}

string SocketAddress::toString() const
{
    return ::to_string(getSockaddr(), getSockaddrLen());
}

tuple<std::string, uint16_t> SocketAddress::getAddressPort() const
{
    return ::get_address_port(getSockaddr(), getSockaddrLen());
}

sockaddr *SocketAddress::getSockaddr() const 
{
    return (sockaddr *)&addr_;
}

socklen_t SocketAddress::getSockaddrLen() const 
{
    return addrLen_;
}

NetworkLayerType SocketAddress::getNetworkLayerType() const
{
    return ::get_network_layer_type(getSockaddr(), getSockaddrLen());
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

bool Socket::open(NetworkLayerType version, TransportLayerType type, const std::nothrow_t &nothrow_value)
{
    return createSocket(static_cast<int>(version), static_cast<int>(type), 0, nothrow_value);
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
        throw_socket_exception("Fetch of local address failed (getsockname())");
    }

    return SocketAddress((sockaddr *)&addr, addrLen);
}

void Socket::createSocket(int domain, int type, int protocol)
{
    if (isOpened())
        close();

    sockDesc_ = socket(domain, type, protocol);
    if (!isOpened()) {
        throw_socket_exception("Can't create socket");
    }
}

bool Socket::createSocket(int domain, int type, int protocol, const std::nothrow_t &nothrow_value)
{
    if (isOpened())
        close();

    sockDesc_ = socket(domain, type, protocol);
    return isOpened();
}

void Socket::bind(const SocketAddress &localAddress)
{
    if (::bind(sockDesc_, localAddress.getSockaddr(), localAddress.getSockaddrLen()) != 0) {
        throw_socket_exception("bind error");
    }
}

// CommunicatingSocket 
void CommunicatingSocket::connect(const SocketAddress &foreignAddress)
{
    if (::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen()) != 0) {
        throw_socket_exception("connect error");
    }
}

void CommunicatingSocket::connect(const SocketAddressView &foreignAddress)
{
    if (::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen()) != 0) {
        throw_socket_exception("connect error");
    }
}

bool CommunicatingSocket::connect(const SocketAddress &foreignAddress, const std::nothrow_t &nothrow_value)
{
    int ret = ::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    return (ret == 0);
}

bool CommunicatingSocket::connect(const SocketAddressView &foreignAddress, const std::nothrow_t &nothrow_value)
{
    int ret = ::connect(sockDesc_, foreignAddress.getSockaddr(), foreignAddress.getSockaddrLen());
    return (ret == 0);
}

int CommunicatingSocket::send(const char *buffer, int bufferLen)
{
    int n = ::send(sockDesc_, buffer, bufferLen, 0);
    if ( n < 0 ) {
        throw_socket_exception("Send failed (send())");
    }

    return n;
}

int CommunicatingSocket::recv(char *buffer, int bufferLen)
{
    int n = ::recv(sockDesc_, buffer, bufferLen, 0); 
    if ( n < 0 ) {
        throw_socket_exception("Receive failed (recv())");
    }

    return n;
}

SocketAddress CommunicatingSocket::getForeignAddress() const
{
    sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getpeername(sockDesc_, (sockaddr *) &addr, &addrLen) != 0) {
        throw_socket_exception("Fetch of foreign address failed (getpeername())");
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
        throw_socket_exception("listen error");
    }
}

shared_ptr<TCPSocket> TCPServerSocket::accept()
{
    SOCKET newConnSD;
    if ((newConnSD = ::accept(sockDesc_, NULL, 0)) == INVALID_SOCKET) {
        throw_socket_exception("Accept failed (accept())");
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
        throw_socket_exception("Send failed (sendto())");
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
        throw_socket_exception("Receive failed (recvfrom())");
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
            throw_socket_exception("Disconnect failed (connect())");
    }
}

// DNSResolver 
DNSResolver::Iterator DNSResolver::query(const char *host, const char *serv, addrinfo *hints)
{
    addrinfo *res;
    int n;
    if ( (n = getaddrinfo(host, serv, hints, &res)) != 0) {
        throw_gai_exception(n);
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
