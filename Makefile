# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpetrov <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/06 09:37:16 by dpetrov           #+#    #+#              #
#    Updated: 2017/12/06 13:37:27 by dpetrov          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= 21sh

CC		= -gcc
CFLAGS	= -Wall -Wextra -Werror

SRCS	= main.c ft_loop.c execute.c launch.c \
		  ft_get_args.c check_dollar_1.c check_dollar_2.c err.c sig.c \
		  parser/quotes.c parser/wildcard.c \
		  parser/home_symbol.c parser/parser.c \
		  builtins/env.c builtins/echo.c builtins/cd.c builtins/exit.c \
		  builtins/setenv.c builtins/history.c \
		  free_mem/free_2d.c free_mem/free_1d.c free_mem/free_the_chain.c \
		  \
		  21sh/tty.c 21sh/ft_readline.c 21sh/detect_ctrl.c 21sh/exec_ctrl.c \
		  21sh/init.c

OBJS	= $(SRCS:.c=.o)

SRC_PATH	= srcs/
OBJ_PATH	= objs/
INCDIR		= -I includes/ -I libft/includes

SRC 	= $(addprefix $(SRC_PATH), $(SRCS))
OBJ		= $(addprefix $(SRC_PATH), $(SRCS))

LFLAGS	= -Llibft -lft -ltermcap

.PHONY: all clean fclean re

all: lib $(NAME)

lib:
	@make -C libft/

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(INCDIR) -o $(NAME) $(OBJ) $(LFLAGS)
	@echo "\033[32m21SH:\t\t:BUILDED [ ✔ ]\033[0m"

$(OBJ_PATH)%.o : $(SRC_PATH)%.c includes/*.h
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

clean:
	@make -C libft/ clean
	@rm -rf $(OBJ_PATH)
	@echo "\033[0;33m21SH:\t\t:CLEANED\033[0m"

fclean: clean
	@make -C libft/ fclean
	@rm -f $(NAME)
	@echo "\033[0;33m21SH:\t\t:FUlL CLEANED\033[0m"

re: fclean all

dev:
	@echo "\033[32m\t--> dpetrov <--\033[0m"
