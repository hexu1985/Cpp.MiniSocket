RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -std=c++11
LIBS = -lpthread
VPATH = ../common

PROGS =	daytimeudpcli daytimeudpsrv daytimeudpcli_byname daytimeudpsrv_byname

all:	${PROGS}

daytimeudpcli:	daytimeudpcli.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

daytimeudpsrv:	daytimeudpsrv.o err_quit.o sock_ntop.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

daytimeudpcli_byname:	daytimeudpcli_byname.o udp_connect.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

daytimeudpsrv_byname:	daytimeudpsrv_byname.o udp_server.o err_quit.o sock_ntop.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o

distclean:
		rm -f ${PROGS} ${CLEANFILES} *.o
