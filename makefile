CC       := gcc
CFLAGS   := -Wall -Wextra -I include -I lib -MMD -MP
 
TARGET   := build/main
 
# Source files
SRCS     := src/main.c \
			src/poly.c \
			libmontmul/montmul.c
 
# Object files (all go into build/)
OBJS     := $(patsubst %.c,build/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)
 
.PHONY: all test clean
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@

 
# Compile any .c to build/*.o
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -rf build
 
-include $(DEPS)