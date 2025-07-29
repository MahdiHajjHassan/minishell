CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = minishell

SRC_DIR = src
INC_DIR = inc
LIBFT_DIR = inc/libft
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: clean-files $(NAME)

clean-files:
	@rm -f files.txt

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -o $@ $(OBJS) -L$(LIBFT_DIR) -lft

# Special rule for runner.c to disable infinite recursion warning
$(OBJ_DIR)/runner.o: $(SRC_DIR)/runner.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -Wno-infinite-recursion -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# General rule for other object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f files.txt
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re clean-files
