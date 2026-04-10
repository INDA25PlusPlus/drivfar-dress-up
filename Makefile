CC = gcc
PKG_CONFIG ?= pkg-config
CSFML_PKGS := csfml-graphics csfml-window csfml-system csfml-audio

# Use pkg-config if available and the packages exist.
HAVE_CSFML_PC := $(shell command -v $(PKG_CONFIG) >/dev/null 2>&1 && \
    $(PKG_CONFIG) --exists $(CSFML_PKGS) && echo yes)

ifeq ($(HAVE_CSFML_PC),yes)
  CFLAGS += $(shell $(PKG_CONFIG) --cflags $(CSFML_PKGS))
  LDFLAGS += $(shell $(PKG_CONFIG) --libs $(CSFML_PKGS))
else
  # TODO: Maybe don't assume homebrew is used?
  # Can be overriden by adding an environment variable
  CSFML_PREFIX ?= /opt/homebrew
  CFLAGS += -I$(CSFML_PREFIX)/include
  LDFLAGS += -L$(CSFML_PREFIX)/lib $(addprefix -l,$(CSFML_PKGS))
endif

CFLAGS += -Isrc -Wall

TARGET_EXEC := program
# The test runner executable.
TESTER_EXEC := build/tester

SRC_DIRS := src
TESTS_DIR := tests
BUILD_DIR := build
ASSETS_DIR := assets

# recursively find all src files
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
HEADERS := $(shell find $(SRC_DIRS) -name '*.h')
# create corresponding build paths
OBJS := $(SRCS:$(SRC_DIRS)/%.c=$(BUILD_DIR)/%.o)
OBJS_NOT_MAIN := $(filter-out $(BUILD_DIR)/main.o, $(OBJS))

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	@echo "Linking $(TARGET_EXEC)..."
	$(CC) $^ $(LDFLAGS) -o $@
	@test -f $@ && echo "Build successful"

# TODO: They currently don't depend on any *.h files.
$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

## Test framework

# The test object files.
TEST_OBJS := $(foreach src,$(shell find $(TESTS_DIR) -name '*.c'),$(src:$(TESTS_DIR)/%.c=$(BUILD_DIR)/tests/%.o))

$(TESTER_EXEC): $(TEST_OBJS) $(OBJS_NOT_MAIN)
	$(CC) $^ $(LDFLAGS) -o $@

# TODO: They currently don't depend on any *.h files.
$(BUILD_DIR)/tests/%.o: $(TESTS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

## Other targets

clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(TARGET_EXEC)
	@test ! -f $(BUILD_DIR) $(TARGET) && echo "Done cleaning"

fmt:
	clang-format -i $(SRCS) $(HEADERS)

check-fmt:
	clang-format --dry-run --Werror -i $(SRCS) $(HEADERS)

.PHONY: all clean check-deps fmt check-fmt
