fast:
	clang++ -pthread -O3 -ffast-math -std=c++11 src/*.c src/*.cpp -o bin/bzo

test:
	clang++ -pthread -O1 -std=c++11 src/*.c src/*.cpp -o bin/bzo
