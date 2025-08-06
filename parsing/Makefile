CC        = gcc
RM        = rm -rf
NAME      = parser
SRCS      = parser.c ../get_next_line/get_next_line_bonus.c \
		    ../get_next_line/get_next_line_utils_bonus.c

OBJS      = $(SRCS:.c=.o)
CFLAGS    = -Wall -Wextra -Werror

# libft varible
LIBFT     = ../libft/libft.a 
DIR_LIBFT = ../libft

all: $(NAME)
 
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)


$(LIBFT):
	make -C $(DIR_LIBFT)

clean:
	$(RM) $(OBJS)
	make clean -C $(DIR_LIBFT)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(DIR_LIBFT)

re: fclean all

.PHONY: all re clean fclean
.SECONDARY: $(OBJS)