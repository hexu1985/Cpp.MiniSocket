set(MINI_SOCKET_LIB mini_socket-static)

add_executable(hostent hostent.cpp)
target_link_libraries(hostent ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

install(TARGETS hostent 
    DESTINATION samples/names)

file(GLOB TEST_SCRIPTS *.sh)
install(FILES ${TEST_SCRIPTS}
    DESTINATION samples/names)

