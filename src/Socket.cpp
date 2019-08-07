#include "Socket.hpp"
#include "SYSException.hpp"

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
