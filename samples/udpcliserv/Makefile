
OS ?= $(shell uname -s)

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -DNDEBUG
INCLUDES = -I../../include
LDFLAGS = -lpthread
LDPATH =
VPATH = ../../src

ifeq ($(OS), Windows_NT)
	CXXFLAGS += -D_WIN32_WINNT=0x0600 # _WIN32_WINNT for inet_ntop
	LDFLAGS = -lwsock32 -lws2_32 #-lpthread 
endif

PROGS =	udpcli udpserv 

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	@echo "OS: $(OS)"
	$(RM) $(PROGS) *.o

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(CXXFLAGS) -o $@ $^

udpcli:	udpcli.o dg_cli.o mini_socket.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

udpserv:	udpserv.o dg_echo.o mini_socket.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

