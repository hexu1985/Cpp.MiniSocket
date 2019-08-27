/** \example names/hostent.cpp
 * This is an example of how to use the DNSResolver class to resolve domain name.
 */
#include <iostream>
#include "mini_socket.hpp"

using namespace std;
using namespace mini_socket;

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        cout << "resolve dns of " << argv[i] << endl;
        try {
            DNSResolver resolver;
            for (auto addr: resolver.query(argv[i], NULL, TransportLayerType::TCP)) {
                cout << "\taddress: " << get<0>(addr.getAddressPort()) << endl;
            }
        } catch(const SocketException &e) {
            cout << e.what() << endl;
        }
    }

    return 0;
}
