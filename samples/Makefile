
OS ?= $(shell uname -s)

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -DASIO_STANDALONE 
INCLUDES = -I../include
LDFLAGS = -lpthread
LDPATH =

ifeq ($(OS), Windows_NT)
	CXXFLAGS += -D_WIN32_WINNT=0x0600 # _WIN32_WINNT for inet_ntop
	LDFLAGS = -lwsock32 -lws2_32 #-lpthread 
endif

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	@echo "OS: $(OS)"
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ ../src/mini_socket.cpp $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH) 
