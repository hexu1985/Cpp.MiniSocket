RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -std=c++11
LIBS = -lpthread
VPATH = ../common

PROGS =	hostent hostent2 hostent3 hostent4 prmyaddrs

all:	${PROGS}

hostent:	hostent.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

hostent2:	hostent2.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

hostent3:	hostent3.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

hostent4:	hostent4.o sock_ntop.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

prmyaddrs:	prmyaddrs.o sock_ntop.o myaddrs.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
