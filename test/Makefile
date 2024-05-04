CC = gcc
CFLAGS = -Wall -Werror -Wextra

NAME = run_test
INC = -I../includes/ -I../Unity/src/
OBJ_DIR = ./obj/
SRC_UNITY = ../Unity/src/unity.c
GENERATE_RUNNER = ../Unity/auto/generate_test_runner.rb
TEST ?= 
RUNNER = runner.c

SRC := $(TEST) 
SRC += test_$(notdir $(basename $(TEST))).c
SRC += $(SRC_UNITY)
SRC += runner.c
OBJ = $(patsubst %.c,$(OBJ_DIR)%.o,$(notdir $(SRC)))

all: fclean generate_runner $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $@ 
	./$(NAME)

generate_runner:
	@echo "Generando Runner para $(TEST)"
	@$(GENERATE_RUNNER) test_$(notdir $(basename $(TEST))).c $(RUNNER)

fclean: 
	@rm -f $(NAME) $(RUNNER)

print: 
	@echo $(SRC)
	@echo $(OBJ)

help:
	@echo "make TEST=src_file_to_check"

.PHONY: test
