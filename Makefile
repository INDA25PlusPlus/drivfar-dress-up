CC = gcc
PKG_CONFIG ?= pkg-config
CSFML_PKGS := csfml-graphics csfml-window csfml-system

CFLAGS += $(shell $(PKG_CONFIG) --cflags $(CSFML_PKGS))
LDLIBS += $(shell $(PKG_CONFIG) --libs $(CSFML_PKGS))

TARGET_EXEC := program

SRC_DIRS := src
BUILD_DIR := build
ASSETS_DIR := assets

# recursively find all src files
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
# create corresponding build paths
OBJS := $(SRCS:$(SRC_DIRS)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET_EXEC)


$(TARGET_EXEC): $(OBJS)
	@echo "Linking $(TARGET_EXEC)..."
	@$(CC) $^ $(LDLIBS) -o $@
	@test -f $@ && echo "Build successful"

$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(TARGET_EXEC)
	@test ! -f $(BUILD_DIR) $(TARGET) && echo "Done cleaning"
	

.PHONY: all clean check-deps