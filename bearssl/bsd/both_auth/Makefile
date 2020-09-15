
RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I../bearssl/inc
LDFLAGS = -lbearssl
LDPATH = -L../bearssl/build

SOURCES = $(shell ls *.c)
PROGS = $(subst .c,,$(SOURCES))
#PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

%: %.c
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
