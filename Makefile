NAME = woody_woodpacker

CC = gcc
CFLAGS = -Wall -Werror -Wextra
FSANITIZER = -g -fsanitize=address 

LIBFT_DIR = ./libft42/
LNK = -L $(LIBFT_DIR) -lft
SRC_DIR = ./src/
OBJ_DIR = ./obj/
INC_DIR = ./includes/
INC = -I$(INC_DIR)

#SRC_FILES
include ./src_list

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))
LIBFT = $(addprefix $(LIBFT_DIR), libft.a)

all: obj $(LIBFT) $(NAME)
	rm -Rf woody
	rm -Rf woody2

$(LIBFT):
	@make -C $(LIBFT_DIR)	

obj:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -g $(INC) -I$(LIBFT_DIR) -o $@ -c $<

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LNK) -o $@ $(FSANITIZER)

print:
	@echo $(SRC)
	@echo ""
	@echo $(OBJ)

clean:
	@rm -Rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all
