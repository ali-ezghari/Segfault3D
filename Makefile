GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
GREEN_CHECK = \033[32m✅\033[0m

CC = cc
RM = rm -rf
FLAGS = #-Wall -Wextra -Werror
SRCS = src/main.c src/utils.c src/rendering.c src/raycasting.c src/init.c \
       parsing/parser.c parsing/parser_utils.c get_next_line/get_next_line_bonus.c \
       get_next_line/get_next_line_utils_bonus.c parsing/parser_utils_two.c \
       parsing/check_map.c parsing/init_color_dir.c parsing/parser_utils_three.c

OBJS = $(SRCS:.c=.o)
NAME = cub3d

# libft varible
LIBFT = libft/libft.a 
DIR_LIBFT = libft

# Include paths
INCLUDES = -Iincludes -Ilibft -Iget_next_line -Iparsing

all: $(NAME)

%.o: %.c
	@$(CC) $(FLAGS) $(INCLUDES) -O3 -c $< -o $@
	@echo "$(GREEN)Compiled $< $(GREEN_CHECK)$(RESET)"

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT) -Lminilibx-linux -lmlx_Linux -lXext -lX11 -lm -o $(NAME)
	@echo "$(GREEN)Mandatory: $(NAME) was created $(GREEN_CHECK)$(RESET)"

$(LIBFT):
	@$(MAKE) -C $(DIR_LIBFT)
	@echo "$(GREEN)Libft was created $(GREEN_CHECK)$(RESET)"

clean:
	@$(RM) $(OBJS)
	@$(MAKE) clean -C $(DIR_LIBFT)
	@echo "$(RED)Removing object files...$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) fclean -C $(DIR_LIBFT)
	@echo "$(RED)Removing $(NAME)...$(RESET)"

re: fclean all

.PHONY: all clean fclean re