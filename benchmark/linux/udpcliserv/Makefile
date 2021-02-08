RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -std=c++11
LIBS = -lpthread
VPATH = ../common

PROGS =	udpcli udpserv udpserv_byname udpcli_byname 

all:	${PROGS}

udpcli:	udpcli.o dg_cli.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

udpserv: udpserv.o dg_echo.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

udpserv_byname: udpserv_byname.o udp_server.o dg_echo.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

udpcli_byname:	udpcli_byname.o udp_connect.o dg_cli.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o

distclean:
		rm -f ${PROGS} ${CLEANFILES} *.o
