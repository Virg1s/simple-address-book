#################################
## VARIABLES ####################
#################################

CC := gcc
CFLAGS := -Wall -Wextra

SRCDIR := src
BUILDDIR := build
LIB := -L lib
INC := -I include

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
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB) -O3

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

#################################
## CLEANUP   ####################
#################################

.PHONY: clean
clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

