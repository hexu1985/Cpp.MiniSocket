set(UNP_LIB unp-static)

add_executable(tcpcli tcpcli.cpp str_cli.cpp)
target_link_libraries(tcpcli ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(tcpserv tcpserv.cpp str_echo.cpp)
target_link_libraries(tcpserv ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(tcpserv_reuseaddr tcpserv_reuseaddr.cpp str_echo.cpp)
target_link_libraries(tcpserv_reuseaddr ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(tcpserv_select tcpserv_select.cpp)
target_link_libraries(tcpserv_select ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(tcpserv_poll tcpserv_poll.cpp)
target_link_libraries(tcpserv_poll ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(tcpserv_fork tcpserv_fork.cpp str_echo.cpp)
target_link_libraries(tcpserv_fork ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(tcpserv_byname tcpserv_byname.cpp str_echo.cpp)
target_link_libraries(tcpserv_byname ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(tcpcli_byname tcpcli_byname.cpp str_cli.cpp)
target_link_libraries(tcpcli_byname ${UNP_LIB} ${LIBS_SYSTEM})

