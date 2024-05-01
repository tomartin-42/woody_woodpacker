CC = gcc
CFLAGS = -Wall -Werror -Wextra

$NAME = run_test
INC = -I../includes/ -I../Unity/src/
OBJ_DIR = ./obj/
SRC_UNITY = ../Unity/src/unity.c
GENERATE_RUNNER = ../Unity/auto/generate_test_runner.rb
TEST ?= 
RUNNER = runner.c

SRC := $(TEST) 
SRC += test_$(TEST)
SRC += $(SRC_UNITY)
SRC += runner.c
OBJ = $(patsubst %.c,$(OBJ_DIR)%.o,$(notdir $(SRC)))

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC)%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $@ $(OBJ)

obj:
	mkdir -p $(OBJ_DIR)

generate_runner: $(GENERATE_RUNNER)
	@echo "Generando Runner para $(TEST)"
	@$(GENERATE_RUNNER) $(TEST) $(RUNNER)

clean:
	@rm -Rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(RUNNER)

print: 
	@echo $(SRC)
	@echo $(OBJ)

help:
	@echo "make test TEST=src_file_to_check"

.PHONY: test
