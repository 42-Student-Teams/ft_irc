# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/24 12:13:57 by inaranjo          #+#    #+#              #
#    Updated: 2024/05/02 15:20:34 by inaranjo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRC_DIR = Sources
SRCS = $(wildcard $(SRC_DIR)/*.cpp main.cpp)
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address
INCLUDES = -I ./Includes

RM = rm -rf

BUILD_DIR   = .build

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅Executable $(NAME) ready.$(RESET)"
	
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) $(BUILD_DIR)
	@echo "$(YELLOW)❌...Removing object files...$(RESET)"
	@echo "Object files removed.$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(YELLOW)❌...Removing executable $(NAME)...$(RESET)"
	@echo "Executable $(NAME) removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
