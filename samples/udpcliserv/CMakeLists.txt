set(MINI_SOCKET_LIB mini_socket-static)

add_executable(udpcli udpcli.cpp dg_cli.cpp)
target_link_libraries(udpcli ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

add_executable(udpcli_byname udpcli_byname.cpp dg_cli_connected.cpp)
target_link_libraries(udpcli_byname ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

add_executable(udpserv udpserv.cpp dg_echo.cpp)
target_link_libraries(udpserv ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

install(TARGETS udpcli udpserv udpcli_byname 
    DESTINATION samples/udpcliserv)

file(GLOB TEST_SCRIPTS *.sh)
install(FILES ${TEST_SCRIPTS}
    DESTINATION samples/udpcliserv)

