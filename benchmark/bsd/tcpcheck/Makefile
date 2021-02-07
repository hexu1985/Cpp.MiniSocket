RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O0 ${INCLUDE}
LIBS = 
VPATH = ../common

PROGS =	tcpcheck tcpcheck_timeo tcpcheck_standalone

all:	${PROGS}

tcpcheck:	tcpcheck.o tcp_connect.o sock_ntop_host.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpcheck_timeo:	tcpcheck_timeo.o tcp_connect_timeo.o connect_nonb.o sock_ntop_host.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpcheck_standalone:	tcpcheck_standalone.o sock_ntop_host.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o

distclean:
		rm -f ${PROGS} ${CLEANFILES} *.o
