all: cmake_config
	cmake --build ./build

cmake_config:
	cmake -S ./src -B ./build

run: all
	./build/pacman

pack: xstola03-xbezak02.zip

xstola03-xbezak02.zip: src Makefile Doxyfile README.md examples
	zip -9 -r $@ $^

doc:
	doxygen Doxyfile

clean:
	rm -rf ./build

.PHONY: doc run cmake_config all clean pack

