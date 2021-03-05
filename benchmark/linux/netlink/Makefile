RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -std=c++11
LIBS = -lpthread
VPATH = ../common

PROGS =	netlink_listen

all:	${PROGS}

netlink_listen:	netlink_listen.o err_quit.o sock_ntop.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
