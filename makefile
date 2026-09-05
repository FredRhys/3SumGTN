CC = gcc
LD = $(CC)
CFLAGS = -std=c23\
		 -Wall\
		 -O3\
 		 -pedantic
# 		 -g\
# 		 -fsanitize=address\
# 		 -fsanitize=undefined

buildfiles := build build/main.o build/basic.o factor64/factor64.o build/advanced.o build/poly.o build/montmul.o build/wrappers.o build/formula.o

build/main: $(buildfiles)
	$(LD) $(CFLAGS) -fopenmp $(wildcard build/*.o) factor64/factor64.o -o build/main -lm -lprimesieve

build: 
	mkdir build

build/main.o: src/main.c build/basic.o factor64/factor64.o build/advanced.o build/poly.o build/montmul.o build/wrappers.o build/formula.o
	$(LD) $(CFLAGS) -fopenmp -c src/main.c -o build/main.o

build/basic.o: src/basic.c factor64/factor64.o
	$(LD) $(CFLAGS) -c src/basic.c -o build/basic.o

factor64/factor64.o:
	cd factor64 && $(MAKE) factor64.o

build/advanced.o: src/advanced.c build/poly.o build/montmul.o build/wrappers.o build/formula.o
	$(LD) $(CFLAGS) -c src/advanced.c -o build/advanced.o

build/poly.o: src/poly.c build/montmul.o
	$(LD) $(CFLAGS) -c src/poly.c -o build/poly.o

build/wrappers.o: src/wrappers.c build/montmul.o
	$(LD) $(CFLAGS) -c src/wrappers.c -o build/wrappers.o

build/montmul.o: libmontmul/montmul.c
	$(LD) $(CFLAGS) -c libmontmul/montmul.c -o build/montmul.o

build/formula.o: src/formula.c
	$(LD) $(CFLAGS) -c src/formula.c -o build/formula.o

.PHONY: clean
clean:
	rm -rf $(wildcard build/*)

.PHONY: run
run: run.sh
	bash run.sh

run.sh:
	echo "build/main 1000000 12" > run.sh
	chmod a+x run.sh

default: build/main
