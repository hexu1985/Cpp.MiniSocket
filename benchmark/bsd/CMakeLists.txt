CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
project(mini_socket)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_definitions(-DNDEBUG)

if(WIN32)
    message(FATAL_ERROR "not support windows platform")
else()
	set(LIBS_SYSTEM c stdc++ pthread)
endif()

include_directories(common)

add_subdirectory(common)
add_subdirectory(daytimetcp)
add_subdirectory(daytimeudp)
add_subdirectory(names)
add_subdirectory(tcpcliserv)
add_subdirectory(udpcliserv)
add_subdirectory(ioctl)
