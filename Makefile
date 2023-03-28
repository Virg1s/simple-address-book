CFLAGS = -Wall -Wextra
OBJECTS = address_book.o linked_list.o contact.o
BIN = address_book
FINAL = ${BIN}

.PHONY: all
all: address_book

.PHONY: fresh
fresh: clean all

.PHONY: debug
debug: CFLAGS += -ggdb3
debug: fresh
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${FINAL} > /dev/null

.PHONY: vim
vim: debug
	./${FINAL}

.PHONY: clean
clean:
	$(RM) ${OBJECTS} ${BIN}

address_book: ${OBJECTS}
