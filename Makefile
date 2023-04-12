GREEN = \033[92m
EOC = \033[0m

NAME		=		clusterize

CC 			=		c++

CFLAGS 		=		-Wall -Wextra -Werror

SRCS		=		srcs/clusterize.cpp	

OBJ			=		$(SRCS:.cpp=.o)

%.o:				%.cpp
					$(CC) $(CFLAGS) -c $(SRCS)
					@mv *o srcs/
					@echo "$(GREEN)compiling srcs..$(EOC)"

all:				$(NAME)

$(NAME):        $(OBJ)
				$(CC) $(CFLAGS) $(OBJ) main.cpp -o $(NAME)
				@echo "$(GREEN)objects to binary..$(EOC)"

clean:
				rm -rf $(OBJ)

fclean:			clean
				rm -rf $(NAME)

re:				fclean all

.PHONY:			clean fclean all re