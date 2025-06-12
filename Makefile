NAME		= webserv

CC			= c++ 
CFLAGS		= -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address
DEP_FLAG	= -MMD -MP
INCLUDE		= -I$(INC_DIR)

RMF			= rm -f
RMD			= rm -rf
MKDIR		= mkdir -p

INC_DIR		= inc/
SRC_DIR		= src/
OBJ_DIR		= obj/

SRC_FILES	= main HttpRequest

SRCS		= $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILES)))
OBJS		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
DEPS		= $(addprefix $(OBJ_DIR), $(addsuffix .d, $(SRC_FILES)))



all:	$(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp Makefile
		$(MKDIR) $(dir $@)
		$(CC) $(CFLAGS) $(INCLUDE) $(DEP_FLAG) -c $< -o $@

clean:
	$(RMD) $(OBJ_DIR)

fclean: clean
	$(RMF) $(NAME)

re: fclean all

-include $(DEPS)
.PHONY:	all clean fclean re


