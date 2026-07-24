CC       := gcc
CFLAGS   := -Wall -Wextra -I include -I lib -MMD -MP
 
TARGET   := main
 
# Source files
SRCS     := $(wildcard src/*.c)
LIB_SRCS := $(wildcard lib/*.c)
 
# Object files (all go into build/)
OBJS     := $(patsubst %.c,build/%.o,$(SRCS) $(LIB_SRCS))
TEST_OBJS := $(patsubst %.c,build/%.o,$(TEST_SRCS))
DEPS     := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)
 
.PHONY: all test clean
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@
 
test: $(TEST_BIN)
	./$(TEST_BIN)
 
$(TEST_BIN): $(TEST_OBJS) $(filter-out build/src/main.o,$(OBJS))
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@
 
# Compile any .c to build/*.o
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -rf build
 
-include $(DEPS)