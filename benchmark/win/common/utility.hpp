#ifndef UNP_UTILITY_INC
#define UNP_UTILITY_INC

#include <string>

inline
std::string WSAGetLastErrorString()
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

#endif
