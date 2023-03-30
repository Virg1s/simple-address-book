#################################
## VARIABLES ####################
#################################

CC := gcc
CFLAGS := -Wall -Wextra -c
AR := ar
ARFLAGS := rcs
DYNAMIC_LIB_FLAGS := -fPIC

LIB_FLAG := -L ..
INCL_FLAG := -I ../../include

SRCDIR := .
STATIC_BUILD_DIR := static_objects

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
STATIC_OBJECTS := $(patsubst $(SRCDIR)/%,$(STATIC_BUILD_DIR)/%,$(SOURCES:.$(SRCEXT)=.o))
STATIC_TARGET := ../liblist.a
DYNAMIC_TARGET := ../liblist.so

#################################
## TARGETS   ####################
#################################

.PHONY: all
all: ${DYNAMIC_TARGET}

.PHONY: fresh
fresh: clean all

.PHONY: debug
debug: CFLAGS += -ggdb3
debug: fresh

#################################
## GENERICS  ####################
#################################

$(DYNAMIC_TARGET): $(SRCDIR)/*.$(SRCEXT)
	pwd
	@echo " Building dynamic lib..."
	$(CC) $(CFLAGS) $(INCL_FLAG) $(DYNAMIC_LIB_FLAGS) -o $@ $^

$(STATIC_TARGET): $(STATIC_OBJECTS)
	@echo " Archiving..."
	$(AR) $(ARFLAGS) $(STATIC_TARGET) $(OBJECTS)

$(STATIC_BUILD_DIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building static objects..."
	@mkdir -p $(STATIC_BUILD_DIR)
	$(CC) $(CFLAGS) $(INCL_FLAG) -c -o $@ $<

#################################
## CLEANUP   ####################
#################################

.PHONY: clean
clean:
	@echo " Cleaning..."; 
	$(RM) -r $(STATIC_BUILD_DIR) $(STATIC_TARGET) $(DYNAMIC_TARGET)

