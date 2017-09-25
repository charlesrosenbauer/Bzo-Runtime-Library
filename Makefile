all:
	clang++ -pthread -std=c++11 src/*.c src/*.cpp -o bin/bzo
