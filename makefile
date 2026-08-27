CC = gcc
LD = $(CC)
CFLAGS = -std=c23\
		 -Wall\
		 -O3\
		 -pedantic
# 		 -g\
# 		 -fsanitize=address\
# 		 -fsanitize=undefined\

build/main: build build/main.o factor64/factor64.o build/basic.o
	$(LD) $(CFLAGS) -fopenmp build/main.o factor64/factor64.o build/basic.o -o build/main -lm

build:
	mkdir build

build/main.o: src/main.c
	$(LD) $(CFLAGS) -fopenmp -c src/main.c -o build/main.o

factor64/factor64.o:
	cd factor64 && $(MAKE) factor64.o

build/basic.o:  src/basic.c
	$(LD) $(CFLAGS) -c src/basic.c -o build/basic.o

.PHONY: clean
clean:
	rm -rf $(wildcard build/*)

default: build/main
