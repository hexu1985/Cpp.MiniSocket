set(MINI_SOCKET_LIB mini_socket-static)

add_executable(sample_socket_address sample_socket_address.cpp)
target_link_libraries(sample_socket_address ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

