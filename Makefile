# AdvCAD Main Makefile - Simple and Economic
# Uses the existing proven Makefile system

.PHONY: all clean lib app test

all: lib app

# Build the WH library
lib:
	$(MAKE) -C src/lib

# Build the advcad application  
app: lib
	$(MAKE) -C src/apps/advcad

# Run regression tests
test: app
	cd tests/regression && ./run_tests.sh

# Clean all build artifacts
clean:
	$(MAKE) -C src/lib clean
	$(MAKE) -C src/apps/advcad clean
	rm -rf build/*

# Install (copy to standard location)
install: app
	cp src/apps/advcad/advcad /usr/local/bin/ 2>/dev/null || cp src/apps/advcad/advcad ~/bin/

help:
	@echo "AdvCAD Build System"
	@echo "=================="
	@echo "make all     - Build library and application"
	@echo "make lib     - Build WH library only"
	@echo "make app     - Build advcad application"
	@echo "make test    - Run regression tests"
	@echo "make clean   - Clean all build artifacts"
	@echo "make install - Install advcad to system"