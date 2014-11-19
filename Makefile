CC = g++ -std=c++0x
CC_OPTS = -O0 -g -c
LINKER = g++ -std=c++0x

LINKSTATE = linkstate
LINKSTATE_OBJS = linkstate.o

DISTVEC = distvec
DISTVEC_OBJS = distvec.o

all:$(LINKSTATE) $(DISTVEC)

$(LINKSTATE): $(LINKSTATE_OBJS)
	$(LINKER) -o $(LINKSTATE) $(LINKSTATE_OBJS)
$(DISTVEC): $(DISTVEC_OBJS)
	$(LINKER) -o $(DISTVEC) $(DISTVEC_OBJS)

linkstate.o: linkstate.h linkstate.cpp
	$(CC) $(CC_OPTS) linkstate.cpp

distvec.o: distvec.h distvec.cpp
	$(CC) $(CC_OPTS) distvec.cpp

clean:
	-rm linkstate distvec
