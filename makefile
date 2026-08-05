CC       := gcc
CFLAGS   := -Wall -Wextra -I include -I lib -MMD -MP -O3 -fopenmp
 
TARGET   := build/main
 
# Source files
SRCS     := src/main.c \
			src/basic.c\
			src/poly.c \
			libmontmul/montmul.c\
			factor64/factor64.c
 
# Object files (all go into build/)
OBJS     := $(patsubst %.c,build/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)
 
.PHONY: all test clean
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ -lm

 
# Compile any .c to build/*.o
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -lm
 
clean:
	rm -rf build
 
-include $(DEPS)