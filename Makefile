NAME		=	ft_irc

SRC			=	srcs/main.cpp \
				srcs/server/Server.cpp \
				srcs/users/Users.cpp \
				srcs/channels/Channels.cpp \
				srcs/commands/parseCommands.cpp \
				srcs/commands/join.cpp \
				srcs/commands/nick.cpp \
				srcs/commands/oper.cpp \
				srcs/commands/kick.cpp \
				srcs/commands/invite.cpp \
				srcs/commands/part.cpp \
				srcs/commands/topic.cpp \
				srcs/commands/mode.cpp \
				srcs/commands/user.cpp \
				srcs/commands/pass.cpp \
				srcs/commands/message.cpp \
				srcs/commands/ping.cpp \
				srcs/commands/notice.cpp \
				srcs/commands/quit.cpp \
				srcs/commands/cap.cpp \

HEADERS		=	includes/*.hpp

OBJ			=	$(SRC:srcs/%.cpp=objs/%.o)

CC			=	g++
RM			=	rm -f

CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic #-fsanitize=address -g3

OBJDIR		=	objs/

all:			dirs $(NAME)

$(NAME):		$(OBJ)
				$(CC) $(CPPFLAGS) -I includes $(OBJ) -o $(NAME)

objs/%.o:		srcs/%.cpp $(HEADERS)
				$(CC) $(CPPFLAGS) -I includes -c $< -o $@

dirs:
				mkdir -p $(OBJDIR)
				mkdir -p objs/server/
				mkdir -p objs/users/
				mkdir -p objs/channels/
				mkdir -p objs/commands/

clean:
				$(RM) -r $(OBJDIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re dirs
