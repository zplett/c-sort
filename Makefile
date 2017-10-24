all: sort

sort.o: sort.c
	gcc -c sort.c

sort: sort.o
	gcc sort.o -o sort

clean:
	rm -rf *.o sort
