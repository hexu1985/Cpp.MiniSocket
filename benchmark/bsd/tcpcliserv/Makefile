RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -std=c++11
LIBS = -lpthread
VPATH = ../common

PROGS =	tcpcli tcpserv tcpserv_reuseaddr tcpserv_select tcpserv_poll tcpcli_select tcpserv_fork \
		tcpserv_byname tcpcli_byname tcpcli_rcvtimeo tcpserv_rcvtimeo

all:	${PROGS}

tcpcli:	tcpcli.o str_cli.o send_all.o recv_until.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpserv: tcpserv.o str_echo.o send_all.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpserv_reuseaddr: tcpserv_reuseaddr.o str_echo.o send_all.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpserv_select: tcpserv_select.o send_all.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpserv_poll: tcpserv_poll.o send_all.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpcli_select: tcpcli.o str_cli_select.o send_all.o recv_until.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpserv_fork: tcpserv_fork.o str_echo.o send_all.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpserv_byname: tcpserv_byname.o tcp_listen.o str_echo.o send_all.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpcli_byname:	tcpcli_byname.o tcp_connect.o str_cli.o send_all.o recv_until.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpcli_rcvtimeo:	tcpcli_rcvtimeo.o str_cli.o send_all.o recv_until.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

tcpserv_rcvtimeo: tcpserv_rcvtimeo.o str_echo.o send_all.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o

distclean:
		rm -f ${PROGS} ${CLEANFILES} *.o
