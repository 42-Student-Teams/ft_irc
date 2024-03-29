# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/24 12:13:57 by inaranjo          #+#    #+#              #
#    Updated: 2024/03/24 12:18:31 by inaranjo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = $(wildcard src/*.cpp src/scc/*.cpp src/command/*.cpp)
OBJS = $(SRCS:.cpp=.o)

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDES = -I ./includes

RM = rm -rf

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

SRCDIR = src
OBJDIR = .objFiles
INCDIR = includes

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅Executable $(NAME) ready.$(RESET)"
	
%.o: %.cpp
	@mkdir -p $(@D)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(YELLOW)❌...Removing object files...$(RESET)"
	@$(RM) $(OBJS)
	@echo "Object files removed.$(RESET)"

fclean: clean
	@echo "$(YELLOW)❌...Removing executable $(NAME)...$(RESET)"
	@$(RM) $(NAME)
	@echo "Executable $(NAME) removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
