set(MINI_SOCKET_LIB mini_socket-static)

add_executable(daytimetcpcli daytimetcpcli.cpp)
target_link_libraries(daytimetcpcli ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

add_executable(daytimetcpsrv daytimetcpsrv.cpp)
target_link_libraries(daytimetcpsrv ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

install(TARGETS daytimetcpcli daytimetcpsrv 
    DESTINATION samples/daytimetcp)

file(GLOB TEST_SCRIPTS *.sh)
install(FILES ${TEST_SCRIPTS}
    DESTINATION samples/daytimetcp)

