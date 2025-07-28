/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/wait.h>
# include <ctype.h>

/* Signal handling structure */
typedef struct s_sig
{
	volatile sig_atomic_t	sigint;
	volatile sig_atomic_t	sigquit;
	volatile pid_t			pid;
	volatile int			exit_status;
}	t_sig;

extern t_sig	g_sig;

/* Command Types - Used to identify different command structures */
# define EXEC	1
# define REDIR	2
# define PIPE	3
# define LIST	4
# define BACK	5

/* Maximum number of arguments allowed in a command */
# define MAXARGS	100

/* Structure to hold redirection parameters */
typedef struct s_redir_params
{
	int	mode;
	int	fd;
}	t_redir_params;

/* Structure to hold process arguments parameters */
typedef struct s_process_args_params
{
	struct s_execcmd	*cmd;
	char				**input_ptr;
	char				*input_end;
	int					*argc;
}	t_process_args_params;

/* Structure to hold token processing parameters */
typedef struct s_token_params
{
	char	*s;
	char	*input_end;
	char	*input_ptr;
	char	*symbols;
	char	*space;
}	t_token_params;

/* Structure to hold process default case parameters */
typedef struct s_process_default_params
{
	char	**s_ptr;
	char	*input_end;
	char	*input_ptr;
	char	*symbols;
	char	*space;
}	t_process_default_params;

/* Structure to hold environment variable handling parameters */
typedef struct s_env_var_params
{
	const char	*str;
	size_t		*i;
	char		**result;
	size_t		*j;
	size_t		*alloc_size;
}	t_env_var_params;

/* Structure to hold regular character handling parameters */
typedef struct s_regular_char_params
{
	const char	*str;
	size_t		*i;
	char		**result;
	size_t		*j;
	size_t		*alloc_size;
}	t_regular_char_params;

/* Structure to hold process character parameters */
typedef struct s_process_char_params
{
	const char	*str;
	size_t		len;
	size_t		*i;
	char		**result;
	size_t		*j;
	size_t		*alloc_size;
}	t_process_char_params;

/* Global variable to store the exit status of the last command */
extern int		g_last_exit_status;

/*
 * Base command structure
 * All other command structures start with this to allow type-based casting
 */
typedef struct s_cmd
{
	int	type;
}	t_cmd;

/*
 * Structure for simple command execution
 * Example: ls -l /home
 */
typedef struct s_execcmd
{
	int		type;
	char	*av[MAXARGS];
	char	*eav[MAXARGS];
}	t_execcmd;

/*
 * Structure for input/output redirection
 * Examples: ls > output.txt, cat < input.txt
 */
typedef struct s_redircmd
{
	int				type;
	struct s_cmd	*cmd;
	char			*file;
	char			*efile;
	int				mode;
	int				fd;
}	t_redircmd;

/*
 * Structure for command pipelines
 * Example: ls -l | grep .txt | wc -l
 */
typedef struct s_pipecmd
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	t_pipecmd;

/*
 * Structure for command lists (commands separated by semicolon)
 * Example: ls ; pwd ; echo hello
 */
typedef struct s_listcmd
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	t_listcmd;

/*
 * Structure for background commands
 * Example: sleep 100 &
 */
typedef struct s_backcmd
{
	int				type;
	struct s_cmd	*cmd;
}	t_backcmd;

/* Basic shell functions */
char			*readline_helper(void);
void			*get_cwd(char *buf, size_t size);
void			wtf(void);
int				forkk(void);
void			runcmd(struct s_cmd *cmd);
struct s_cmd	*tokenize(const char *line);
struct s_cmd	*nulterm(struct s_cmd *cmd);
char			*find_command(const char *cmd);

/* Main helper functions */
void			display_prompt(void);
void			init_signals(void);
int				handle_line_input(char **line);
int				handle_tokenize(char *line, struct s_cmd **cmd);
void			expand_builtin_args(struct s_execcmd *ecmd);
int				handle_builtin_cmd(struct s_cmd *cmd, char *line);
void			handle_child_status(int status);
void			execute_cmd(struct s_cmd *cmd);

/* Parser and command constructor functions */
struct s_cmd	*parseexec(char **input_ptr, char *input_end);
int				peek(char **input_ptr, char *input_end, char *toks);
int				gettoken(char **input_ptr, char *input_end,
					char **token_start, char **token_end);

/* Tokenize helper functions */
int				is_escaped(const char *s, const char *start);
char			*skip_whitespace(char *s, char *input_end, char *space);
int				handle_basic_symbols(char **s_ptr, char *input_ptr);
int				handle_greater_than(char **s_ptr, char *input_ptr);
int				handle_special_chars(char **s_ptr, char *input_ptr);
char			*handle_default_token(char *s, char *input_end, char *input_ptr,
					t_token_params params);
void			init_space_array(char *space);
void			init_symbols_array(char *symbols);
void			init_token_arrays(char *space, char *symbols);
t_token_params	setup_token_params(char *s, char *input_end,
					t_token_params params);
int				process_default_case(char **s_ptr, char *input_end,
					t_process_default_params params);
void			setup_token_processing(char **input_ptr, char *input_end,
					char **token_start, char **s_ptr);
int				handle_token_cases(char **s_ptr, char *input_ptr,
					char *input_end);

/* Env helper functions */
size_t			get_var_name_len(const char *str);
char			*get_env_value(const char *name, size_t name_len);
char			*init_result_buffer(size_t len, size_t *alloc_size);
char			*resize_for_env_value(char *result, size_t *alloc_size,
					size_t j, size_t value_len);
int				handle_env_variable(t_env_var_params params);
char			*resize_for_char(char *result, size_t *alloc_size, size_t j);
int				handle_regular_char(t_regular_char_params params);
int				is_variable_char(const char *str, size_t i, size_t len);
int				process_character(t_process_char_params params);

/* Parses helper2 functions */
char			get_escape_char(char c);
void			handle_escape_sequence(const char *input, size_t *i,
					char *output, size_t *j);
void			remove_redir_quotes(char **q, char **eq);
struct s_cmd	*apply_input_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*apply_output_redir(struct s_cmd *cmd, char *file);

/* Parses helper3 functions */
struct s_cmd	*apply_append_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*init_exec_cmd(void);
int				get_exec_token(char **input_ptr, char *input_end, char **q,
					char **eq);
void			remove_exec_quotes(char **q, char **eq);
char			*process_argument(char *q, char *eq);

/* Parses helper4 functions */
void			add_argument(struct s_execcmd *cmd, char *processed, int *argc);
void			finalize_exec_cmd(struct s_execcmd *cmd, int argc);
struct s_cmd	*process_arguments(struct s_cmd *ret,
					t_process_args_params params);

/* Utils helper functions */
void			handle_redir_case(struct s_cmd *cmd);
void			handle_pipe_case(struct s_cmd *cmd);
void			handle_list_case(struct s_cmd *cmd);
void			handle_back_case(struct s_cmd *cmd);
void			free_exec_cmd(struct s_execcmd *ecmd);
void			free_redir_cmd(struct s_redircmd *rcmd);
void			free_pipe_cmd(struct s_pipecmd *pcmd);
void			free_list_cmd(struct s_listcmd *lcmd);
void			free_back_cmd(struct s_backcmd *bcmd);
void			free_cmd(struct s_cmd *cmd);

/* Runner helper2 functions */
size_t			get_path_segment_len(char *curr, char **next);
int				build_full_path(char *full_path, char *curr,
					size_t len, const char *cmd);
void			reset_signals(void);
void			handle_exec_builtin(struct s_execcmd *ex, struct s_cmd *cmd);
int				open_redir_file_create(struct s_redircmd *rdir);

/* Runner helper3 functions */
void			open_redir_file_regular(struct s_redircmd *rdir);
void			handle_list_builtin(struct s_execcmd *ex);
void			handle_list_external(struct s_cmd *cmd);
void			process_list_left(struct s_listcmd *list);
void			process_list_right(struct s_listcmd *list);

/* Runner helper4 functions */
void			setup_pipe_left(int *p, struct s_pipecmd *pipecmd);
void			setup_pipe_right(int *p, struct s_pipecmd *pipecmd);
void			run_back_cmd(struct s_cmd *cmd);

/* Tokenize helper2 functions */
int				handle_basic_symbols(char **s_ptr, char *input_ptr);
int				handle_greater_than(char **s_ptr, char *input_ptr);
void			init_space_array(char *space);
void			init_symbols_array(char *symbols);
void			init_token_arrays(char *space, char *symbols);

/* Tokenize helper3 functions */
t_token_params	setup_token_params(char *s, char *input_end,
					t_token_params params);
int				process_default_case(char **s_ptr, char *input_end,
					t_process_default_params params);
void			setup_token_processing(char **input_ptr, char *input_end,
					char **token_start, char **s_ptr);

/* Env helper1 helper functions */
char			*init_result_buffer(size_t len, size_t *alloc_size);
char			*resize_for_env_value(char *result, size_t *alloc_size,
					size_t j, size_t value_len);
char			*resize_for_char(char *result, size_t *alloc_size, size_t j);
int				is_variable_char(const char *str, size_t i, size_t len);

/* Main helper1 helper functions */
int				handle_eof(char *buf, size_t len);
void			handle_backspace(size_t *len);
char			*resize_buffer(char *buf, size_t *capacity);
char			*init_buffer(size_t *capacity, size_t *len);
int				handle_eof_char(char **buf, size_t *len);
int				handle_error_char(char **buf);
int				handle_newline_char(char **buf, size_t *len);
int				handle_backspace_char(size_t *len);
int				handle_char_input(char **buf, size_t *capacity, size_t *len, int c);
int				process_char(int c, char **buf, size_t *capacity, size_t *len);

struct s_cmd	*pipecmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd	*backcmd(struct s_cmd *subcmd);
struct s_cmd	*listcmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd	*redircmd(struct s_cmd *subcmd, char *file,
					char *efile, t_redir_params params);
struct s_cmd	*execcmd(void);
struct s_cmd	*parse_redirs(struct s_cmd *cmd,
					char **input_ptr, char *input_end);
struct s_cmd	*parse_block(char **input_ptr, char *input_end);
struct s_cmd	*parse_line(char **input_ptr, char *input_end);

/* Parser helper functions */
char			*init_output_buffer(size_t len);
char			get_escape_char(char c);
void			handle_escape_sequence(const char *input, size_t *i,
					char *output, size_t *j);
char			*process_escaped(const char *input, size_t len);
int				get_redir_token(char **input_ptr, char *input_end, char **q,
					char **eq);
void			remove_redir_quotes(char **q, char **eq);
char			*process_filename(char *q, char *eq);
struct s_cmd	*apply_input_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*apply_output_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*apply_append_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*handle_redir_token(struct s_cmd *cmd, int tok, char *file);
struct s_cmd	*init_exec_cmd(void);
int				get_exec_token(char **input_ptr, char *input_end, char **q,
					char **eq);
void			remove_exec_quotes(char **q, char **eq);
char			*process_argument(char *q, char *eq);
void			add_argument(struct s_execcmd *cmd, char *processed, int *argc);
void			finalize_exec_cmd(struct s_execcmd *cmd, int argc);
struct s_cmd	*process_arguments(struct s_cmd *ret,
					t_process_args_params params);

/* Built-in command handling */
int				is_builtin(char *cmd);
int				handle_builtin(char **argv);
char			*expand_variables(const char *str, size_t len);
void			set_exit_status(int status);

/* Builtin helper functions */
int				cd_to_home(void);
int				cd_to_path(char *path);
int				parse_export_arg(char *arg, char **name, char **value);
void			remove_quotes(char **value);
int				set_environment_var(char *name, char *value);

/* Builtin command functions */
int				builtin_echo(char **argv);
int				builtin_cd(char **argv);
int				builtin_pwd(char **argv);
int				builtin_exit(char **argv);

/* Runner helper functions */
char			*check_absolute_path(const char *cmd);
size_t			get_path_segment_len(char *curr, char **next);
int				build_full_path(char *full_path, char *curr, size_t len,
					const char *cmd);
char			*search_in_paths(char *path, const char *cmd);
void			reset_signals(void);
void			expand_exec_args(struct s_execcmd *ex);
void			handle_exec_builtin(struct s_execcmd *ex, struct s_cmd *cmd);
void			execute_external_cmd(struct s_execcmd *ex);
int				open_redir_file_create(struct s_redircmd *rdir);
void			open_redir_file_regular(struct s_redircmd *rdir);
void			handle_list_builtin(struct s_execcmd *ex);
void			handle_list_external(struct s_cmd *cmd);
void			process_list_left(struct s_listcmd *list);
void			process_list_right(struct s_listcmd *list);
void			setup_pipe_left(int *p, struct s_pipecmd *pipecmd);
void			setup_pipe_right(int *p, struct s_pipecmd *pipecmd);
void			run_pipe_cmd(struct s_cmd *cmd);
void			run_back_cmd(struct s_cmd *cmd);

#endif