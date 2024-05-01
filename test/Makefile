C_COMPILER = gcc
CFLAGS = -Wall -Werror -Wextra

INC = -I../includes/ -I../Unity/src/
SRC_DIR = ../src/
OBJ_DIR = ./obj/
SRC_UNITY = ../Unity/src/unity.c


#SRC_FILES
include ../src_list
#SRC_TEST
include src_test

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES)) $(SRC_UNITY) $(SRC_TEST)
OBJ = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o)) 
OBJ += $(addprefix $(OBJ_DIR), $(SRC_TEST:.c=.o))

obj:
	mkdir -p $(OBJ_DIR)

print:
	@echo $(SRC)
	@echo $(OBJ)
