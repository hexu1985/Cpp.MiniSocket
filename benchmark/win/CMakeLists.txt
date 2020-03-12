CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
project(mini_socket)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_definitions(-DNDEBUG)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /utf-8")
endif()

if(WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	set(LIBS_SYSTEM ws2_32)
else()
    message(FATAL_ERROR "only support windows platform")
endif()

include_directories(common)

add_subdirectory(common)
add_subdirectory(daytimetcp)
add_subdirectory(daytimeudp)
add_subdirectory(names)
add_subdirectory(tcpcliserv)
add_subdirectory(udpcliserv)
