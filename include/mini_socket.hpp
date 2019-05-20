#ifndef PNL_MINI_SOCKET_INC
#define PNL_MINI_SOCKET_INC

#if defined WIN32 or defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#endif

#include <cstdint>
#include <string>
#include <tuple>
#include <stdexcept>
#include <memory>
#include <iostream>

#if defined WIN32 or defined _WIN32
namespace MiniSocket {

class _WSAStartupHolder_;

class _WSAStartupSharedHolder_ {
public:
    _WSAStartupSharedHolder_(); 
    ~_WSAStartupSharedHolder_(); 

private:
    std::shared_ptr<_WSAStartupHolder_> sharedHolder_;
};

static _WSAStartupSharedHolder_ _wsa_startup_shared_holder_;

}   // namespace MiniSocket
#else   // not WIN32

typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

#endif

namespace MiniSocket {

// SocketException
class SocketException : public std::runtime_error {
public:
  SocketException(const std::string &message); 
  
  SocketException(const std::string &message, const std::string &detail);
};

// SocketAddressView
class SocketAddressView {
public:
    static std::tuple<std::string, uint16_t> getAddressPort(const sockaddr *sa, socklen_t salen);
    static std::string toString(const sockaddr *sa, socklen_t salen);

public:
    SocketAddressView(const sockaddr *addrVal = NULL, socklen_t addrLenVal = 0);

    std::string toString() const;

    std::tuple<std::string, uint16_t> getAddressPort() const;

    const sockaddr *getSockaddr() const; 

    socklen_t getSockaddrLen() const; 

private:
    const sockaddr *addr_ = 0;
    socklen_t addrLen_ = 0;
};

// SocketAddress 
class SocketAddress {
public:
    SocketAddress(); 
    SocketAddress(const char *address, uint16_t port); 
    SocketAddress(sockaddr *addrVal, socklen_t addrLenVal);

    bool setAddressPort(const char *address, uint16_t port); 

    std::string toString() const;

    std::tuple<std::string, uint16_t> getAddressPort() const;

    sockaddr *getSockaddr() const; 

    socklen_t getSockaddrLen() const; 

private:
    sockaddr_storage addr_;
    socklen_t addrLen_ = sizeof (sockaddr_storage);
};

// Socket
class Socket {
public:
    virtual ~Socket();

    void createSocket(int domain, int type, int protocol);

    void closeSocket();

    bool isValid() const;

    SocketAddress getLocalAddress() const;

    void bind(const SocketAddress &localAddress);

private:
    Socket(const Socket &sock) = delete;
    void operator=(const Socket &sock) = delete;

protected:
    SOCKET sockDesc_ = INVALID_SOCKET;

    Socket();
};

// CommunicatingSocket 
class CommunicatingSocket : public Socket {
public:
    void connect(const SocketAddress &foreignAddress);
    bool connect(const SocketAddress &foreignAddress, const std::nothrow_t &nothrow_value);

    size_t send(const char *buffer, int bufferLen); 

    size_t recv(char *buffer, int bufferLen); 

    SocketAddress getForeignAddress() const;
};

class TCPSocket : public CommunicatingSocket {
public:
    TCPSocket();
    TCPSocket(const SocketAddress &foreignAddress); 

    void sendAll(const char *buffer, int bufferLen); 

    std::iostream &getStream(); 

private:
    friend class TCPServerSocket;
    TCPSocket(SOCKET sockDesc);

    std::iostream *myStream_ = 0;
    std::streambuf *myStreambuf_ = 0;
};

class TCPServerSocket : public Socket {
public:
    TCPServerSocket(const SocketAddress &localAddress); 

	void listen(int backlog);

    std::shared_ptr<TCPSocket> accept();
};

}   // MiniSocket

#endif
