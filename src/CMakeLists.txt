file(GLOB MINI_SOCKET_LIB_SRC_LIST *.cpp)

add_library(mini_socket SHARED ${MINI_SOCKET_LIB_SRC_LIST})
target_link_libraries(mini_socket ${LIBS_SYSTEM}) 

add_library(mini_socket-static STATIC ${MINI_SOCKET_LIB_SRC_LIST})

install(TARGETS mini_socket mini_socket-static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
