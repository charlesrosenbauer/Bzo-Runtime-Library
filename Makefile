all:
	clang++ -pthread -O3 -std=c++11 src/*.c src/*.cpp -o bin/bzo
