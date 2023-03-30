#################################
## VARIABLES ####################
#################################

CC := gcc
CFLAGS := -Wall -Wextra
LDLIBS := -llist

SRCDIR := src
BUILDDIR := build
LIBDIR := lib

INCL_FLAG := -I include
LDFLAGS := -L lib

SRCEXT := c

SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TARGET = bin/address_book

#################################
## TARGETS   ####################
#################################

.PHONY: all
all: ${TARGET}

.PHONY: fresh
fresh: clean all

.PHONY: debug
debug: CFLAGS += -ggdb3
debug: fresh
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${TARGET} > /dev/null

.PHONY: vim
vim: debug
	./${TARGET}

.PHONY: vgd
vgd: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${TARGET}

#################################
## GENERICS  ####################
#################################

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building..."
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCL_FLAG) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LDLIBS) $(LDFLAGS)

#################################
## CLEANUP   ####################
#################################

.PHONY: clean
clean:
	@echo " Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET)

