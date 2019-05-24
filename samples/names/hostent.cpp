/** \example hostent.cpp
 * This is an example of how to use the DNSResolver class to resolve domain name.
 */
#include <iostream>
#include "mini_socket.hpp"

using namespace std;
using namespace MiniSocket;

int main(int argc, char *argv[])
{
    DNSResolver resolver;
    SocketAddressView addr;
    for (int i = 1; i < argc; i++) {
        cout << "resolve dns of " << argv[i] << endl;
        try {
            auto iter = resolver.query(argv[i], NULL, TransportLayerType::TCP);
            while (iter.hasNext()) {
                addr = iter.next();
                cout << "\taddress: " << get<0>(addr.getAddressPort()) << endl;
            }
        } catch(const SocketException &e) {
            cout << e.what() << endl;
        }
    }

    return 0;
}
