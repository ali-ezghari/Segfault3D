GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
GREEN_CHECK = \033[32m✅\033[0m

CC = cc
FLAGS = #-Wall -Wextra -Werror
SRCS = src/main.c src/utils.c src/rendering.c src/raycasting.c src/init.c
# SRCSB =
OBJS = $(SRCS:.c=.o)
# OBJSB = $(SRCSB:.c=.o)
NAME = cub3d

all: $(NAME)

%.o: %.c
	@$(CC) $(FLAGS) -O3 -c $< -o $@
	@echo "$(GREEN)Compiled $< $(GREEN_CHECK)$(RESET)"

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -Lminilibx-linux -lmlx_Linux -lXext -lX11 -lm -o $(NAME)
	@echo "$(GREEN)Mandatory: $(NAME) was created $(GREEN_CHECK)$(RESET)"
	
clean:
	@rm -f $(OBJS) $(OBJSB)
	@echo "$(RED)Removing object files...$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)Removing $(NAME)...$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus
