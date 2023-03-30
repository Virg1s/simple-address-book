BIN:=addr_book
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)

CPPFLAGS:=-I../include
CFLAGS:=-Wall
LDFLAGS:=-L../lib
LDLIBS:=-llist

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o ../bin/$@ $^ $(LDFLAGS) $(LDLIBS)

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) ../bin/$(BIN) $(OBJS)