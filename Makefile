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
HEADERS := $(shell find $(SRC_DIRS) -name '*.h')
# create corresponding build paths
OBJS := $(SRCS:$(SRC_DIRS)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET_EXEC)


$(TARGET_EXEC): $(OBJS)
	@echo "Linking $(TARGET_EXEC)..."
	$(CC) $^ $(LDLIBS) -o $@
	@test -f $@ && echo "Build successful"

# TODO: They currently don't depend on any *.h files.
$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(TARGET_EXEC)
	@test ! -f $(BUILD_DIR) $(TARGET) && echo "Done cleaning"

fmt:
	clang-format -i $(SRCS) $(HEADERS)

check-fmt:
	clang-format --dry-run --Werror -i $(SRCS) $(HEADERS)

.PHONY: all clean check-deps fmt check-fmt
