CFLAGS = -Wall -Wextra
objects = address_book.o linked_list.o contact.o
final = address_book

.PHONY: all
all: address_book

.PHONY: fresh
fresh: clean all

.PHONY: debug
debug: CFLAGS += -ggdb3
debug: fresh
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${final} > /dev/null

.PHONY: clean
clean:
	$(RM) $(wildcard *.o)
	$(RM) address_book

address_book: $(objects)
