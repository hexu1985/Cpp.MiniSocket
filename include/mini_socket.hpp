
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

#if defined WIN32 or defined _WIN32
#else
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

namespace MiniSocket {

class SocketException : public std::runtime_error {
public:
  SocketException(const std::string &message); 
  
  SocketException(const std::string &message, const std::string &detail);
};

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
    const sockaddr *addr_;
    socklen_t addrLen_;
};

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
    socklen_t addrLen_;
};

}   // MiniSocket
