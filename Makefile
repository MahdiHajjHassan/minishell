CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = minishell

SRC_DIR = src
INC_DIR = inc
LIBFT_DIR = inc/libft
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $(OBJS)

# Special rule for runner.c to disable infinite recursion warning
$(OBJ_DIR)/runner.o: $(SRC_DIR)/runner.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -Wno-infinite-recursion -I$(INC_DIR) -c $< -o $@

# General rule for other object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
