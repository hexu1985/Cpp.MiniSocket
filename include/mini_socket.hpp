
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


#include <string>
#include <tuple>

std::tuple<std::string, uint16_t> sock_ntop(const struct sockaddr *sa, socklen_t salen);

std::string to_string(const struct sockaddr *sa, socklen_t salen);
