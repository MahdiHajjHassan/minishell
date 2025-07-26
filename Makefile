# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
LIBFT_DIR = ./inc/libft

# Output binary
NAME = minishell

# Source files (recursively all .c files in src/)
SRCS = $(shell find $(SRC_DIR) -name '*.c')

# Object files (obj/foo.o corresponding to src/foo.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(NAME)

# Link final binary
$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJS) -I$(INC_DIR) -L$(LIBFT_DIR) -lft -o $(NAME)

# Compile each source to object
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Compile libft
$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

# Remove object files
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

# Remove binary and object files
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Recompile everything
re: fclean all

.PHONY: all clean fclean re bonus
