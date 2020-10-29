CC := g++
CFLAGS := \
	-Iinclude \
	-Wpedantic \
	-g -Wall \
	-std=c++17
SOURCES = $(wildcard *.cc)
OBJ = ${SOURCES:.cc=.o}

all: run
	./a.out

run: ${OBJ}
	${CC} -o a.out ${OBJ}

%.o: %.cc
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm *.out *.o

memcheck:
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./a.out