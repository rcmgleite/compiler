all: build

debug: add_debug_flags build

add_debug_flags:
	export DEBUG=true

build:
	mkdir -p build
	cd build && cmake .. && make -j5 && cp compiler ..

# Clean Target
clean:
	rm -rf build compiler

.PHONY: all debug add_debug_flags build clean
