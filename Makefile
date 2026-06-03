# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/12 17:31:43 by lciardo           #+#    #+#              #
#    Updated: 2026/06/03 18:16:37 by lciardo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	codexion

# Compilatore e Flag
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -I.

# Sorgenti divisi per cartella per chiarezza
SRC_MAIN	=	main.c
SRC_PARSING	=	parsing/parsing.c

# Unione di tutti i file sorgente
SRCS		=	$(SRC_MAIN) $(SRC_PARSING)

# Trasformazione dei file .c in .o
OBJS		=	$(SRCS:.c=.o)

# Regola principale
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Pulizia oggetti
clean:
	rm -f $(OBJS)

# Pulizia totale
fclean: clean
	rm -f $(NAME)

# Ricompilazione totale
re: fclean all

.PHONY: all clean fclean re