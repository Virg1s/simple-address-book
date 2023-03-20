CC = gcc
CFLAGS = -Wall -Wextra -Werror

.PHONY: all
all: address_book

address_book: address_book.o linked_list.o
	$(CC) $? -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) address_book

