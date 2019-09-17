CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
project(mini_socket)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	set(LIBS_SYSTEM ws2_32)
else()
	set(LIBS_SYSTEM c stdc++ pthread)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)
