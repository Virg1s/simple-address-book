.PHONY: all src lib clean fresh debug

all: lib src

debug: MAKE_FLAGS := debug
debug: all

clean: MAKE_FLAGS := clean
clean: all

fresh: MAKE_FLAGS := fresh
fresh: all

src:
	$(MAKE) $(MAKE_FLAGS) -C ./$@

lib:
	$(MAKE) $(MAKE_FLAGS) -C ./$@

