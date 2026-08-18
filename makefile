CC = gcc
LD = $(CC)
CFLAGS = -std=c23\
		 -g\
		 -Wall\
		 -pedantic\
		 -fsanitize=undefined\
		 -fsanitize=address

.PHONY: main
main: main.o
	$(LD) $(CFLAGS) build/main.o -o build/main

.PHONY: main.o
main.o: src/main.c
	$(LD) $(CFLAGS) -c src/main.c -o build/main.o