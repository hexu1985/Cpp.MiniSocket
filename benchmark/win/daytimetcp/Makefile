RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -D_WIN32_WINNT=0x0600	# _WIN32_WINNT for inet_ntop
LIBS = -lwsock32 -lws2_32
VPATH = ../common

PROGS =	daytimetcpcli daytimetcpsrv daytimetcpcli_byname daytimetcpsrv_byname 

all:	${PROGS}

daytimetcpcli:	daytimetcpcli.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

daytimetcpsrv:	daytimetcpsrv.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

daytimetcpcli_byname:	daytimetcpcli_byname.o tcp_connect.o sock_ntop_host.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

daytimetcpsrv_byname:	daytimetcpsrv_byname.o tcp_listen.o sock_ntop.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o

distclean:
		rm -f ${PROGS} ${CLEANFILES} *.o
