CC = g++ -std=c++0x
CC_OPTS = -O0 -g -c
LINKER = g++ -std=c++0x

LINKSTATE = linkstate
LINKSTATE_OBJS = linkstate.o

all:$(LINKSTATE)

$(LINKSTATE): $(LINKSTATE_OBJS)
	$(LINKER) -o $(LINKSTATE) $(LINKSTATE_OBJS)

linkstate.o: linkstate.h linkstate.cpp
	$(CC) $(CC_OPTS) linkstate.cpp

clean:
	-rm linkstate
