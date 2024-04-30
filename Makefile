# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Probook <Probook@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/24 12:13:57 by inaranjo          #+#    #+#              #
#    Updated: 2024/04/25 14:26:15 by Probook          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# NAME = ircserv

# SRCS = $(wildcard sources/*.cpp *.cpp)
# SRC_DIR = src

# CC = c++
# FLAGS = -Wall -Wextra -Werror -std=c++98
# INCLUDES = -I ./Includes

# RM = rm -rf

# BUILD_DIR   = .build
# # OBJS = $(SRCS:.cpp=.o)
# OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


# # Colors
# GREEN = \033[0;32m
# YELLOW = \033[0;33m
# RESET = \033[0m

# SRCDIR = src
# OBJDIR = .objFiles
# INCDIR = includes

# all: $(NAME)

# $(NAME): $(OBJS)
# 	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
# 	@echo "$(GREEN)✅Executable $(NAME) ready.$(RESET)"
	
# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	@mkdir -p $(@D)
# 	@echo "$(YELLOW)Compiling $<...$(RESET)"
# 	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# clean:
# 	@echo "$(YELLOW)❌...Removing object files...$(RESET)"
# 	@$(RM) $(OBJS) $(BUILD_DIR)
# 	@echo "Object files removed.$(RESET)"

# fclean: clean
# 	@echo "$(YELLOW)❌...Removing executable $(NAME)...$(RESET)"
# 	@$(RM) $(NAME)
# 	@echo "Executable $(NAME) removed.$(RESET)"

# re: fclean all

# .PHONY: all clean fclean re

NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address

SRCS = main.cpp Sources/Commands.cpp Sources/Channel.cpp Sources/Client.cpp Sources/Server.cpp 

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

bonus: $(OBJS_BONUS)
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJS_BONUS)

%.o: %.cpp Includes/Server.hpp Includes/Client.hpp Includes/Channel.hpp Includes/Reply.hpp  Includes/Commands.hpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean re