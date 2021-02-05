RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -std=c++11
LIBS = -lpthread
VPATH = ../common

PROGS =	net_monitor

all:	${PROGS}

net_monitor:	net_monitor.o err_quit.o sock_ntop.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
