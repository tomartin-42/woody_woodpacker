NAME = woody_woodpacker

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC_DIR = ./src/
OBJ_DIR = ./obj/
INC_DIR = ./includes/
INC = -I$(INC_DIR)

#SRC_FILES
include ./src_list

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

all: obj $(NAME)

obj:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $@ $(OBJ)

print:
	echo $(SRC)
	echo $(OBJ)

clean:
	@rm -Rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
