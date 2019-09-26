

NAME = ../corewar

HEAD = ./
SRC = vm.c vm_init.c auxiliary.c parsing.c

OBJ = $(addprefix $(OBJPATH)/,$(SRC:.c=.o))
OBJPATH = ./objects

FLAGS = -g -Wall -Wextra -Werror #-m64 -Ofast -flto -march=native -funroll-loops
LIB = -L ./libft -lft
LIBPATH = ./libft

all: $(NAME)

$(NAME):  $(OBJPATH) $(OBJ)
	@make -C $(LIBPATH)
	gcc $(FLAGS) $(OBJ) -o $(NAME) $(LIB)

$(OBJPATH):
	mkdir -p $(OBJPATH)

$(OBJPATH)/%.o: %.c
	gcc $(FLAGS) -c $< -o $@

clean:
	@make -C $(LIBPATH) clean
	/bin/rm -rf $(OBJPATH)

fclean: clean
	@make -C $(LIBPATH) fclean
	/bin/rm -f $(NAME)

re: fclean all