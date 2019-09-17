file(GLOB MINI_SOCKET_LIB_SRC_LIST *.cpp)

add_library(mini_socket SHARED ${MINI_SOCKET_LIB_SRC_LIST})
add_library(mini_socket-static STATIC ${MINI_SOCKET_LIB_SRC_LIST})


