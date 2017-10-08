all: build

debug: build_debug

create_build_dir:
	mkdir -p build

build: create_build_dir
	cd build && cmake .. && make -j5 && cp compiler ..

build_debug: create_build_dir
	export DEBUG=true && cd build && cmake .. && make -j5 && cp compiler ..

# Clean Target
clean:
	rm -rf build compiler

.PHONY: all debug build clean
