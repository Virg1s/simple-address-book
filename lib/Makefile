LIB:=liblist.so
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
CFLAGS:=-Wall
CPPFLAGS:=-I../include


.PHONY: all debug clean

all: $(LIB)

debug: CFLAGS += -ggdb3
debug: all

$(LIB): $(OBJS)
	$(CC) -o $@ $^ -shared

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fpic -c -o $@ $^

clean:
	$(RM) $(LIB) $(OBJS)

