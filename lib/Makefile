LIB:=liblist.so
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
CFLAGS:=-Wall
CPPFLAGS:=-I../include


.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) -o $@ $^ -shared

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fpic -c -o $@ $^

clean:
	$(RM) $(LIB) $(OBJS)