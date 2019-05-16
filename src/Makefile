
OS ?= $(shell uname -s)

RM = rm -f
AR = ar rcs
CXX = g++
CXXFLAGS = -Wall -g -DNDEBUG -std=c++11
INCLUDES = -I../include
LDFLAGS = 
LDPATH =
VPATH = 

ifeq ($(OS), Windows_NT)
	CXXFLAGS += 
	LDFLAGS = -lwsock32 -lws2_32 #-lpthread 
endif

SOURCES = $(shell ls *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
STATICLIB = libpnl.a

all: $(STATICLIB)
	@echo "STATICLIB = $(STATICLIB)" 

.PHONY: clean
clean:
	$(RM) $(STATICLIB) $(OBJECTS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INCLUDES) $<

$(STATICLIB): $(OBJECTS)
	$(AR) $(STATICLIB) $(OBJECTS)

