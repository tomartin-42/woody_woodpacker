NAME = woody_woodpacker

CC = gcc
ASM = nasm  
ASMFLAGS = -f elf64  
CFLAGS = -Wall -Werror -Wextra
FSANITIZER = -g -fsanitize=address

LIBFT_DIR = ./libft42/
LNK = -L $(LIBFT_DIR) -lft
SRC_DIR = ./src/
OBJ_DIR = ./obj/
INC_DIR = ./includes/
INC = -I$(INC_DIR)

ASM_FILES = asm_encrypt.asm
ASM_SRC = $(addprefix $(SRC_DIR), $(ASM_FILES))
ASM_OBJ = $(addprefix $(OBJ_DIR), $(ASM_FILES:.asm=.o))

include ./src_list
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

LIBFT = $(addprefix $(LIBFT_DIR), libft.a)

all: obj $(LIBFT) $(NAME)
	rm -Rf woody

$(LIBFT):
	@make -C $(LIBFT_DIR)	

obj:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -g $(INC) -I$(LIBFT_DIR) -o $@ -c $<

$(OBJ_DIR)%.o: $(SRC_DIR)%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(NAME): $(OBJ) $(ASM_OBJ)
	$(CC) $(OBJ) $(ASM_OBJ) $(CFLAGS) $(LNK) -o $@ $(FSANITIZER)

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
	@rm -Rf woody

re: fclean all
