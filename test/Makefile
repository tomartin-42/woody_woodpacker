CC = gcc
CFLAGS = -Wall -Werror -Wextra

NAME = run_test
INC = -I../includes/ -I../Unity/src/ -I../libft42/
LNK = -L ../libft42/ -lft
LIBFT_DIR = ../libft42/
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
LIBFT = $(addprefix $(LIBFT_DIR), libft.a)

all: fclean $(LIBFT) generate_runner $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)	

$(NAME): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LNK) $(INC) -o $@ 
	./$(NAME)

generate_runner:
	@echo "Generando Runner para $(TEST)"
	@$(GENERATE_RUNNER) test_$(notdir $(basename $(TEST))).c $(RUNNER)

fclean: 
	@rm -f $(NAME) $(RUNNER)
	@make -C $(LIBFT_DIR) fclean

print: 
	@echo $(SRC)
	@echo $(OBJ)

help:
	@echo "make TEST=src_file_to_check"

.PHONY: test
