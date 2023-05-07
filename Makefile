all: cmake_config
	cmake --build ./build

cmake_config:
	cmake -S ./src -B ./build

run: all
	./build/pacman