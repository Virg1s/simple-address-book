.PHONY: all src lib clean fresh debug

all: lib src
	export LD_LIBRARY_PATH=./lib

fresh: clean all

debug: MAKE_FLAGS = debug
debug: fresh

clean: MAKE_FLAGS = clean
clean: src lib

src:
	$(MAKE) $(MAKE_FLAGS) -C ./$@

lib:
	$(MAKE) $(MAKE_FLAGS) -C ./$@

