CC = cc
CFLAGS = -Wall -Wextra -Werror -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600

NAME = minishell

SRC_DIR = src
INC_DIR = inc
LIBFT_DIR = inc/libft
OBJ_DIR = obj

SRCS = src/builtin.c \
       src/builtin_cd_helper.c \
       src/builtin_cd_utils.c \
       src/builtin_echo_helper.c \
       src/builtin_exit_helper.c \
       src/builtin_export_helper.c \
       src/builtin_helper1.c \
       src/builtin_helper2.c \
       src/builtin_pwd_helper.c \
       src/builtin_utils.c \
       src/constructs.c \
       src/constructs_helper.c \
       src/env.c \
       src/env_helper1.c \
       src/env_helper1_helper.c \
       src/env_helper2.c \
       src/heredoc.c \
       src/heredoc_helper.c \
       src/main.c \
       src/main_helper1.c \
       src/main_helper2.c \
       src/main_helper3.c \
       src/main_helper4.c \
       src/main_helper5.c \
       src/memory_management.c \
       src/memory_management_helper.c \
       src/memory_management_helper2.c \
       src/parses.c \
       src/parses_helper1.c \
       src/parses_helper10.c \
       src/parses_helper2.c \
       src/parses_helper3.c \
       src/parses_helper4.c \
       src/parses_helper4_helper.c \
       src/parses_helper5.c \
       src/parses_helper6.c \
       src/parses_helper6_helper.c \
       src/parses_helper7.c \
       src/parses_helper8.c \
       src/parses_helper9.c \
       src/runner.c \
       src/runner_helper1.c \
       src/runner_helper10.c \
       src/runner_helper11.c \
       src/runner_helper2.c \
       src/runner_helper3.c \
       src/runner_helper4.c \
       src/runner_helper5.c \
       src/runner_helper6.c \
       src/runner_helper7.c \
       src/runner_helper8.c \
       src/runner_helper9.c \
       src/tokenize.c \
       src/tokenize_helper1.c \
       src/tokenize_helper2.c \
       src/tokenize_helper3.c \
       src/utils.c \
       src/utils_helper.c \
       src/utils1.c \
       src/utils1_helper.c \
       src/utils2.c \
       src/utils2_helper.c \
       src/utils3.c \
       src/utils3_helper1.c \
       src/utils3_helper2.c \
       src/utils3_helper3.c \
       src/utils3_helper4.c \
       src/utils3_helper5.c \
       src/utils3_helper6.c \
       src/utils3_helper7.c \
       src/utils3_helper8.c

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -o $@ $(OBJS) -L$(LIBFT_DIR) -lft -lreadline

# General rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
