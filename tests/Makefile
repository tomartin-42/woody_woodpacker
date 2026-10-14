CC = gcc
CFLAGS = -Wall -Wextra -Werror
BIN_DIR = bin
SOURCE = hello_world.c
PIE = $(BIN_DIR)/hello_world_pie
NO_PIE = $(BIN_DIR)/hello_world_no_pie

all: $(PIE) $(NO_PIE)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(PIE): $(SOURCE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -fPIE -pie $< -o $@

$(NO_PIE): $(SOURCE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -fno-pie -no-pie $< -o $@

check: all
	file $(PIE) $(NO_PIE)
	LC_ALL=C readelf -h $(PIE) | grep 'Type:'
	LC_ALL=C readelf -h $(NO_PIE) | grep 'Type:'

clean:
	rm -rf $(BIN_DIR)

.PHONY: all check clean
