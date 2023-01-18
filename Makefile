NAME = philo

SRC =	src/philo.c \
		src/init.c \
		src/utils.c \
		src/args.c \
		src/philo_die.c \
		src/philo_state.c \

OBJ = $(SRC:.c=.o)

CC = gcc

CFLAGS = -Wall -Werror -Wextra -g3
CFLAGS = -g3

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -g -pthread -g -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
	