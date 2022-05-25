# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mde-maul <mde-maul@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/25 16:05:28 by mde-maul          #+#    #+#              #
#    Updated: 2022/05/25 16:05:30 by mde-maul         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minisynth

SRCS = main_margot.c

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Werror -Wextra

INCLUDES = -I include

LIBS = -L ./libft -lft -L lib -l SDL2

all: $(NAME)

$(NAME):
	make -C ./libft
	gcc $(FLAGS) $(INCLUDES) -c $(SRCS)
	gcc $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)

clean:
	make -C ./libft clean
	rm -f $(OBJS)

fclean: clean
	make -C ./libft fclean
	rm -f $(NAME)

re: fclean all

dev:
	rm -f $(NAME)
	gcc $(FLAGS) $(INCLUDES) -o $(NAME) $(SRCS) $(LIBS)
	make clean