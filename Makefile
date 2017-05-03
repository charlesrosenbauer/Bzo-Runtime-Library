all:
	rm bin/bzo.a
	gcc -pthread -fPIC -c src/*.c
	ar ruv bin/*.o -o bin/bzo.a
	ranlib bin/bzo.a
	rm bin/bzo.o bin/tasks.o

test:
	gcc -pthread -fPIC -c src/tasks.c -o bin/tasks.o
	gcc -pthread -fPIC -c src/bzo.c   -o bin/bzo.o
	ar ruv bin/bzo.a bin/bzo.o bin/tasks.o
	ranlib bin/bzo.a
	rm bin/bzo.o bin/tasks.o
	gcc -L bin/bzo.a tests/test.c -o bin/test

testNoLib:
	gcc -pthread src/*.c tests/test.c -o bin/bzo
