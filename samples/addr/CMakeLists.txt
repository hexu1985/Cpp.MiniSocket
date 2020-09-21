set(MINI_SOCKET_LIB mini_socket-static)

add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/test.sh
    COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test.sh
    ${CMAKE_CURRENT_BINARY_DIR}/test.sh
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/test.sh
    )

add_executable(sample_socket_address sample_socket_address.cpp)
target_sources(sample_socket_address PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/test.sh)
target_link_libraries(sample_socket_address ${MINI_SOCKET_LIB} ${LIBS_SYSTEM})

install(TARGETS sample_socket_address 
    DESTINATION samples/addr)

file(GLOB TEST_SCRIPTS *.sh)
install(FILES ${TEST_SCRIPTS}
    DESTINATION samples/addr)

