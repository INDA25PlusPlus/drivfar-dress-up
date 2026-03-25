CC = gcc
# CFLAGS 
LDFLAGS := -lcsfml-graphics  -lcsfml-window -lcsfml-system

TARGET_EXEC := program

SRC_DIRS := src
BUILD_DIR := build
ASSETS_DIR := assets

# recursively find all src files
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
# create corresponding build paths
OBJS := $(SRCS:$(SRC_DIRS)/%.c=$(BUILD_DIR)/%.o)

all: check-deps $(TARGET_EXEC)

check-deps: 
	@pkg-config --exists csfml-graphics csfml-window csfml-system || \
		(echo "Error: CSFML not installed"; exit 1)

$(TARGET_EXEC): $(OBJS)
	@echo "Linking $(TARGET_EXEC)..."
	@$(CC) $^ $(LDFLAGS) -o $@ -l 
	@test -f $@ && echo "Build successful"

$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.c
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(TARGET_EXEC)
	@test ! -f $(BUILD_DIR) $(TARGET) && echo "Done cleaning"
	

.PHONY: all clean check-deps