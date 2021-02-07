RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE}
LIBS = 
VPATH = ../common

PROGS =	tcpcheck tcpcheck_timeo

all:	${PROGS}

tcpcheck:	tcpcheck.o tcp_connect.o sock_ntop_host.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpcheck_timeo:	tcpcheck_timeo.o tcp_connect_timeo.o connect_timeo.o sock_ntop_host.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}
clean:
		rm -f ${PROGS} ${CLEANFILES} *.o

distclean:
		rm -f ${PROGS} ${CLEANFILES} *.o
