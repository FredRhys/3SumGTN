CC = gcc
LD = $(CC)
CFLAGS = -std=c23\
		 -Wall\
		 -Ofast\
		 -pedantic
# 		 -g\
# 		 -fsanitize=address\
# 		 -fsanitize=undefined\

build/main: build build/main.o factor64/factor64.o build/basic.o build/advanced.o
	$(LD) $(CFLAGS) -fopenmp $(wildcard build/*.o) factor64/factor64.o -o build/main -lm -lprimesieve

build:
	mkdir build

build/main.o: src/main.c
	$(LD) $(CFLAGS) -fopenmp -c src/main.c -o build/main.o

build/basic.o: src/basic.c factor64/factor64.o
	$(LD) $(CFLAGS) -c src/basic.c -o build/basic.o

factor64/factor64.o:
	cd factor64 && $(MAKE) factor64.o

build/advanced.o: src/advanced.c build/poly.o build/montmul.o
	$(LD) $(CFLAGS) -c src/advanced.c -o build/advanced.o

build/poly.o: src/poly.o build/montmul.o
	$(LD) $(CFLAGS) -c src/poly.c -o build/poly.o

build/montmul.o: libmontmul/montmul.c
	$(LD) $(CFLAGS) -c libmontmul/montmul.c -o build/montmul.o

.PHONY: clean
clean:
	rm -rf $(wildcard build/*)

default: build/main
