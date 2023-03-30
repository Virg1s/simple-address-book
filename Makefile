
.PHONY: all src lib clean clean-src clean-lib

all: lib src

src:
	$(MAKE) -C ./$@

lib:
	$(MAKE) -C ./$@

clean: clean-src clean-lib

clean-src:
	$(MAKE) -C ./src clean

clean-lib:
	$(MAKE) -C ./lib clean
