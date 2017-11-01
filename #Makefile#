# Detect OS for cflags, indicated when gccx can be used
OS := $(shell uname)

# Establish OS specific commands
ifeq ($(OS), Darwin)
	CC := gcc -g -pedantic -std=c99 -Wall -Wextra
else
	ifeq ($(OS), Linux)
		CC := gccx
	endif
endif
# End of OS specification

all: sort

sort.o: sort.c
	$(CC) -c sort.c

sort: sort.o
	$(CC) sort.o -o sort

clean:
	rm -rf *.o sort
