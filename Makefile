NAME := cqap
STD := -std=c23
CFLAGS := -ftree-parallelize-loops=4 -O2 -march=native
CC := gcc
SRC := $(wildcard src/*.c)
OBJS := $(patsubst %.c, %.o, $(SRC))

BIN_DIR := bin
BUILD_DIR := build

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking..."
	$(CC) $(CFLAGS) $(STD) -o $(BIN_DIR)/qap $(OBJS)

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
	rm build/$(OBJS)

openmp:
	@echo "Compiling using openmp"
	$(CC) $(STD) -o $(BIN_DIR)/qap_openmp -fopenmp $(SRC)
