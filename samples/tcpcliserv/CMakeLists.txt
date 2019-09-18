set(MINI_SOCKET_LIB mini_socket-static)

add_executable(tcpcli tcpcli.cpp str_cli.cpp)
target_link_libraries(tcpcli ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

add_executable(tcpcli_byname tcpcli_byname.cpp str_cli.cpp)
target_link_libraries(tcpcli_byname ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

add_executable(tcpserv tcpserv.cpp str_echo.cpp)
target_link_libraries(tcpserv ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

install(TARGETS tcpcli tcpserv tcpcli_byname 
    DESTINATION samples/tcpcliserv)

file(GLOB TEST_SCRIPTS *.sh)
install(FILES ${TEST_SCRIPTS}
    DESTINATION samples/tcpcliserv)

