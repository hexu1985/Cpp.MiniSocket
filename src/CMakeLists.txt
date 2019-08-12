include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
file(GLOB SRC_LIST *.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (USE_MINI_SOCKET_SHARED)
	add_library(mini_socket SHARED ${SRC_LIST})
else()
	add_library(mini_socket ${SRC_LIST})
endif()


