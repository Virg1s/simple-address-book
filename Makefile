CFLAGS = -Wall -Wextra
objects = address_book.o linked_list.o

.PHONY: all
all: address_book

.PHONY: fresh
fresh: clean all

.PHONY: debug
debug: CFLAGS += -ggdb3
debug: fresh

.PHONY: clean
clean:
	$(RM) $(wildcard *.o)
	$(RM) address_book

address_book: $(objects)
