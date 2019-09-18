set(MINI_SOCKET_LIB mini_socket-static)

add_executable(daytimeudpcli daytimeudpcli.cpp)
target_link_libraries(daytimeudpcli ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

add_executable(daytimeudpsrv daytimeudpsrv.cpp)
target_link_libraries(daytimeudpsrv ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

install(TARGETS daytimeudpcli daytimeudpsrv 
    DESTINATION samples/daytimeudp)

file(GLOB TEST_SCRIPTS *.sh)
install(FILES ${TEST_SCRIPTS}
    DESTINATION samples/daytimeudp)

