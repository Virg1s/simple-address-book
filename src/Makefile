BIN:=../bin/addr_book
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)

CPPFLAGS:=-I../include
CFLAGS:=-Wall
LDFLAGS:=-L../lib
LDLIBS:=-llist

.PHONY: all debug clean fresh

all: $(BIN)

debug: CFLAGS += -ggdb3
debug: clean all

fresh: clean all

$(BIN): $(OBJS)
	mkdir -p ../bin
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) $(BIN) $(OBJS)
