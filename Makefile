CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
NAME = ircserv

SRC = srcs/Client.cpp srcs/Channel.cpp srcs/Message.cpp srcs/Server.cpp main.cpp
	

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re