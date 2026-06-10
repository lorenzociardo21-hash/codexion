# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/03 18:32:53 by lciardo           #+#    #+#              #
#    Updated: 2026/06/10 18:46:39 by lciardo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME    = codexion
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -pthread -I.

OBJ_DIR = obj

SRCS    = main.c \
          parsing.c \
          init.c \
          routine.c \
          controller.c \
          utils.c \
          scheduler.c

OBJS    = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re