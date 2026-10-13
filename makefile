CC = gcc
LD = $(CC)
CFLAGS = -std=c23\
		 -Wall\
		 -O3\
 		 -pedantic\
		 -Ilibdivide\
		 -Ilibmontmul\
		 -Isrc/adv\
		 -Ireturnif/src
#   		 -g
#   	 	 -fsanitize=address\
#  		 -fsanitize=undefined

build/main: build/adv build/main.o
	$(LD) $(CFLAGS) -fopenmp $(wildcard build/*.o build/adv/*.o) factor64/factor64.o -o build/main -lm -lprimesieve

build/adv: build
	@if [ ! -d build/adv ]; then mkdir build/adv; echo "mkdir build/adv"; fi

build: 
	mkdir build

build/main.o: src/main.c returnif/src/returnif.h build/basic.o build/advancedRunner.o
	$(LD) $(CFLAGS) -fopenmp -c src/main.c -o build/main.o

build/basic.o: src/basic.c src/basic.h libdivide/libdivide.h factor64/factor64.o
	$(LD) $(CFLAGS) -c src/basic.c -o build/basic.o

build/advancedRunner.o: src/advancedRunner.c src/advancedRunner.h build/adv/advanced.o build/preliminary.o
	$(LD) $(CFLAGS) -c src/advancedRunner.c -o build/advancedRunner.o

factor64/factor64.o:
	cd factor64 && $(MAKE) factor64.o

build/preliminary.o: src/preliminary.c src/preliminary.h returnif/src/returnif.h build/adv/formula.o
	$(LD) $(CFLAGS) -c src/preliminary.c -o build/preliminary.o

build/adv/advanced.o: src/adv/advanced.c\
 					  src/adv/advanced.h\
  					  libdivide/libdivide.h\
   					  returnif/src/returnif.h\
					  build/adv/poly.o\
					  build/adv/montmul.o\
					  build/adv/wrappers.o\
					  build/adv/formula.o
	$(LD) $(CFLAGS) -c src/adv/advanced.c -o build/adv/advanced.o

build/adv/poly.o: src/adv/poly.c src/adv/poly.h build/adv/montmul.o
	$(LD) $(CFLAGS) -c src/adv/poly.c -o build/adv/poly.o

build/adv/wrappers.o: src/adv/wrappers.c src/adv/wrappers.h build/adv/montmul.o
	$(LD) $(CFLAGS) -c src/adv/wrappers.c -o build/adv/wrappers.o

build/adv/montmul.o: libmontmul/montmul.c libmontmul/montmul.h
	$(LD) $(CFLAGS) -c libmontmul/montmul.c -o build/adv/montmul.o

build/adv/formula.o: src/adv/formula.c src/adv/formula.h libdivide/libdivide.h src/adv/isQr.h
	$(LD) $(CFLAGS) -c src/adv/formula.c -o build/adv/formula.o

.PHONY: clean
clean:
	rm -rf $(wildcard build/* build/adv/*)

.PHONY: run
run: run.sh
	bash run.sh

run.sh:
	echo "build/main 1 100000 4 21 30" > run.sh
	chmod a+x run.sh

default: build/main
