CFLAGS = -ftree-parallelize-loops=4 -std=c23 -O2 -march=native
CC = gcc

all: final

final: main.o qap.o bruteforce.o localsearch.o
	@echo "Linking..."
	$(CC) $(CFLAGS) main.o qap.o bruteforce.o localsearch.o -o qap

main.o: main.c
	@echo "Compiling main.c"
	$(CC) $(CFLAGS) -c main.c

qap.o: qap.c
	@echo "Compiling qap.c"
	$(CC) $(CFLAGS) -c qap.c

bruteforce.o: bruteforce.c
	@echo "Compiling bruteforce.c"
	$(CC) $(CFLAGS) -c bruteforce.c

localsearch.o: localsearch.c
	@echo "Compiling localsearch.c"
	$(CC) $(CFLAGS) -c localsearch.c

clean:
	@echo "Cleaning ALL"
	rm main.o qap.o bruteforce.o localsearch.o

openmp:
	@echo "Compiling using openmp"
	$(CC) -std=c23 -o qap_openmp -fopenmp qap.c main.c bruteforce.c localsearch.c
