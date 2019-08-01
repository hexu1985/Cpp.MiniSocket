
OS ?= $(shell uname -s)

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -DNDEBUG
INCLUDES = -I../../include
LDFLAGS = -lmini_socket -lpthread
LDPATH = -L../../src
#VPATH = ../../src

ifeq ($(OS), Windows_NT)
	CXXFLAGS += -D_WIN32_WINNT=0x0600 # _WIN32_WINNT for inet_ntop
	LDFLAGS = -lmini_socket -lwsock32 -lws2_32 #-lpthread 
endif

PROGS =	tcpcli tcpserv tcpcli_byname

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	@echo "OS: $(OS)"
	$(RM) $(PROGS) *.o

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(CXXFLAGS) -o $@ $^

tcpcli:	tcpcli.o str_cli.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

tcpserv:	tcpserv.o str_echo.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

tcpcli_byname:	tcpcli_byname.o str_cli.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDPATH) $(LDFLAGS)

