NAME = ft_ls

CC = cc

CFLAGS = -Wall -Wextra -Werror -Iincludes -O3 -I$(LIBFT_PATH)/includes -g

SRCS = 	srcs/main.c \
		srcs/color.c \
		srcs/parse_args.c \
		srcs/exec_ls.c \
		srcs/print/print.c  \
		srcs/print/print_long.c  \
		srcs/utils.c \

OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[1;33m
WHITE = \033[0m

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

$(LIBFT):
	@echo -e "$(GREEN)Compiling the Libft ..."
	@make -sC $(LIBFT_PATH)
	@echo -e "$(GREEN)Compiling Complete !$(WHITE)"

clean:
	@echo -e "$(BLUE)Removing objects files ..."
	@make -sC $(LIBFT_PATH) clean
	@rm -rf $(OBJ_DIR)
	@echo -e "$(BLUE)Removing Complete !$(WHITE)"

fclean: clean
	@echo -e "$(YELLOW)Removing archives and executables ..."
	@rm -f $(LIBFT)
	@rm -f $(NAME)
	@echo -e "$(YELLOW)Removing Complete !$(WHITE)"

re: fclean all


.PHONY: all clean fclean re