CC = gcc
LD = $(CC)
CFLAGS = -std=c23\
		 -g\
		 -Wall\
		 -pedantic\
		 -fsanitize=undefined\
		 -fsanitize=address

.PHONY: build/main
build/main: build/main.o factor64/factor64.o
	$(LD) $(CFLAGS) build/main.o factor64/factor64.o -o build/main

.PHONY: build/main.o
build/main.o: src/main.c src/poly.h
	$(LD) $(CFLAGS) -c src/main.c -o build/main.o

.PHONY: factor64/factor64.o
	cd factor64
	make
	cd ..
