# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/24 12:13:57 by inaranjo          #+#    #+#              #
#    Updated: 2024/04/18 17:14:10 by inaranjo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = $(wildcard src/*.cpp src/scc/*.cpp src/command/*.cpp)
SRC_DIR = src

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
INCLUDES = -I ./includes

RM = rm -rf

BUILD_DIR   = .build
# OBJS = $(SRCS:.cpp=.o)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

SRCDIR = src
OBJDIR = .objFiles
INCDIR = includes

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅Executable $(NAME) ready.$(RESET)"
	
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(YELLOW)❌...Removing object files...$(RESET)"
	@$(RM) $(OBJS) $(BUILD_DIR)
	@echo "Object files removed.$(RESET)"

fclean: clean
	@echo "$(YELLOW)❌...Removing executable $(NAME)...$(RESET)"
	@$(RM) $(NAME)
	@echo "Executable $(NAME) removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
