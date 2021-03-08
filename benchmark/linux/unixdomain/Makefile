RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -std=c++11
LIBS = -lpthread
VPATH = ../common

PROGS = acpi_listen acpi_listen2

all:	${PROGS}

acpi_listen:	acpi_listen.o recv_until.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

acpi_listen2:	acpi_listen2.o err_quit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}


clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
