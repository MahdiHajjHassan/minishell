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
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

/* Command Types - Used to identify different command structures */
# define EXEC	1
# define REDIR	2
# define PIPE	3
/* LIST command type removed - semicolon not supported in this minishell */
# define BACK	5
# define HEREDOC	6

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
	char		**env_copy;
}	t_env_var_params;

/* Structure to hold pipe command execution parameters */
typedef struct s_pipe_cmd_params
{
	int					p[2];
	struct s_pipecmd	*pipecmd;
	pid_t				pid1;
	pid_t				pid2;
	int					status1;
	int					status2;
}	t_pipe_cmd_params;

/* Structure to hold format processing parameters */
typedef struct s_format_params
{
	char		*str;
	int			*i;
	int			size;
	const char	**format;
	va_list		args;
}	t_format_params;

/* Structure to hold argument processing parameters */
typedef struct s_arg_process_params
{
	struct s_cmd			**ret;
	t_process_args_params	params;
	char					**env_copy;
	char					**q;
	char					**eq;
}	t_arg_process_params;

/* Structure to hold token type processing parameters */
typedef struct s_token_type_params
{
	struct s_cmd			**ret;
	t_process_args_params	params;
	char					**env_copy;
	int						tok;
	char					**q;
	char					**eq;
}	t_token_type_params;

/* Structure to hold expanded argument processing parameters */
typedef struct s_expanded_arg_params
{
	struct s_cmd			*ret;
	t_process_args_params	params;
	char					**env_copy;
	char					*q;
	char					*eq;
}	t_expanded_arg_params;

/* Structure to hold regular character handling parameters */
typedef struct s_regular_char_params
{
	const char	*str;
	size_t		*i;
	char		**result;
	size_t		*j;
	size_t		*alloc_size;
}	t_regular_char_params;

/* Structure to hold PWD preparation parameters */
typedef struct s_pwd_prep_params
{
	char		*old_pwd;
	char		*new_pwd;
	char		***env_copy;
	char		**old_pwd_var;
	char		**new_pwd_var;
	char		***new_environ;
}	t_pwd_prep_params;

/* Structure to hold process character parameters */
typedef struct s_process_char_params
{
	const char	*str;
	size_t		len;
	size_t		*i;
	char		**result;
	size_t		*j;
	size_t		*alloc_size;
	char		**env_copy;
}	t_process_char_params;

/* Structure to hold redirection token parameters */
typedef struct s_redir_token_params
{
	struct s_cmd			**ret;
	t_process_args_params	params;
	char					**env_copy;
	int						tok;
	char					**q;
	char					**eq;
}	t_redir_token_params;

/* Structure to hold consecutive quotes parameters */
typedef struct s_consecutive_quotes_params
{
	struct s_cmd			**ret;
	t_process_args_params	params;
	char					**env_copy;
	char					**q;
	char					**eq;
}	t_consecutive_quotes_params;

/* Structure to hold redirection file processing parameters */
typedef struct s_redir_file_params
{
	struct s_cmd			**ret;
	t_process_args_params	params;
	char					**env_copy;
	int						tok;
	char					**q;
	char					**eq;
}	t_redir_file_params;

/* Structure to hold redirection command creation parameters */
typedef struct s_redir_cmd_params
{
	struct s_cmd	**ret;
	char			*file_or_delimiter;
	char			**env_copy;
	int				tok;
	int				was_quoted;
}	t_redir_cmd_params;

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
 * REMOVED - semicolon not supported in this minishell
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

/*
 * Structure for heredoc commands
 * Example: cat << EOF
 */
typedef struct s_heredoccmd
{
	int				type;
	struct s_cmd	*cmd;
	char			*delimiter;
	char			*content;
	int				fd;
}	t_heredoccmd;

/* Basic shell functions */
void			*get_cwd(char *buf, size_t size);
void			handle_internal_error(void);
int				forkk(void);
void			handle_exec_case(struct s_cmd *cmd);
int				is_only_whitespace(const char *str);
void			runcmd(struct s_cmd *cmd, char **env_copy);
struct s_cmd	*tokenize(const char *line, char **env_copy);
struct s_cmd	*nulterm(struct s_cmd *cmd);
char			*find_command(const char *cmd, char **env_copy);

/* Main helper functions */
void			init_signals(void);
int				handle_line_input(char **line);
int				handle_tokenize(char *line, struct s_cmd **cmd,
					char **env_copy);
void			expand_builtin_args(struct s_execcmd *ecmd, char **env_copy);
int				handle_builtin_cmd(struct s_cmd *cmd, char *line,
					char ***env_copy);
void			handle_child_status(int status);
void			execute_cmd(struct s_cmd *cmd, char **env_copy);

/* Parser and command constructor functions */
struct s_cmd	*parseexec(char **input_ptr, char *input_end, char **env_copy);
int				peek(char **input_ptr, char *input_end, char *toks);
int				gettoken(char **input_ptr, char *input_end,
					char **token_start, char **token_end);

/* Tokenize helper functions */
int				is_escaped(const char *s, const char *start);
char			*skip_whitespace(char *s, char *input_end, char *space);
int				handle_basic_symbols(char **s_ptr, char *input_ptr);
int				handle_greater_than(char **s_ptr, char *input_ptr);
int				handle_special_chars(char **s_ptr, char *input_ptr);
char			*handle_default_token(char *s, char *input_end,
					char *input_ptr, t_token_params params);
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
char			*get_env_value(const char *name, size_t name_len,
					char **env_copy);
char			*init_result_buffer(size_t len, size_t *alloc_size);
char			*resize_for_env_value(char *result, size_t *alloc_size,
					size_t j, size_t value_len);
int				handle_env_variable(t_env_var_params params, char **env_copy);
char			*resize_for_char(char *result, size_t *alloc_size, size_t j);
int				handle_regular_char(t_regular_char_params params);
int				is_variable_char(const char *str, size_t i, size_t len);
int				process_character(t_process_char_params params);

/* Parses helper2 functions */
char			get_escape_char(char c);
void			handle_escape_char_sequence(const char *input, size_t *i,
					char *output, size_t *j);
void			remove_redir_quotes(char **q, char **eq);
struct s_cmd	*apply_input_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*apply_output_redir(struct s_cmd *cmd, char *file);

/* Parses helper3 functions */
struct s_cmd	*apply_append_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*init_exec_cmd(void);
int				get_exec_token(char **input_ptr, char *input_end, char **q,
					char **eq);
int				remove_exec_quotes(char **q, char **eq);
char			*process_argument(char *q, char *eq);

/* Parses helper4 functions */
void			add_argument(struct s_execcmd *cmd, char *processed, int *argc);
void			finalize_exec_cmd(struct s_execcmd *cmd, int argc);

/* Parses helper5 functions */
char			*allocate_and_copy_result(char *result, char *temp);
char			*process_quoted_segment(char *current_pos, char *input_end,
					char **env_copy, char *result);
char			*skip_whitespace_chars(char *current_pos, char *input_end);

/* Parses helper6 functions */
int				get_exec_token(char **input_ptr, char *input_end, char **q,
					char **eq);
int				remove_exec_quotes(char **q, char **eq);
char			*process_argument(char *q, char *eq);
char			*process_argument_with_expansion(char *q, char *eq,
					char **env_copy, int quote_type);

/* Parses helper7 functions */
char			*handle_quoted_string(char **current_pos, char *input_end,
					char **env_copy, char *result);
char			*initialize_concatenation(char **input_ptr);
char			*process_concatenation_loop(char *result, char *current_pos,
					char *input_end, char **env_copy);
char			*concatenate_quoted_strings(char **input_ptr, char *input_end,
					char **env_copy);
int				process_expanded_argument(t_expanded_arg_params params);

/* Parses helper8 functions */
int				process_exec_token(struct s_cmd *ret,
					t_process_args_params params, char **env_copy);
int				process_single_exec_argument(struct s_cmd *ret,
					t_process_args_params params, char **env_copy);
struct s_cmd	*process_arguments(struct s_cmd *ret,
					t_process_args_params params, char **env_copy);
struct s_cmd	*handle_heredoc_token(struct s_cmd *cmd, char *delimiter,
					char **env_copy, int is_quoted);

/* Parses helper9 functions */
int				validate_filename_token(int filename_tok, struct s_cmd **ret,
					t_process_args_params params);
int				check_quoted_status(char **q, char **eq);
int				create_redirection_cmd(t_redir_cmd_params params);
int				process_redirection_file(t_redir_file_params params);

/* Parses helper10 functions */
int				handle_redirection_token(t_redir_token_params params);
void			skip_to_quote_end(char **temp_ptr, char *temp_end,
					char quote_char);
int				count_consecutive_quotes(char **q, char **eq);
int				process_consecutive_quotes(t_consecutive_quotes_params params);
int				handle_token_type(t_token_type_params token_params);
struct s_cmd	*process_arguments(struct s_cmd *ret,
					t_process_args_params params, char **env_copy);
struct s_cmd	*process_arguments_and_redirs(struct s_cmd *ret,
					t_process_args_params params, char **env_copy);

/* Utils helper functions */
void			handle_redir_case(struct s_cmd *cmd);
void			handle_pipe_case(struct s_cmd *cmd);
void			handle_back_case(struct s_cmd *cmd);
void			handle_heredoc_case(struct s_cmd *cmd);
/* List command handling removed - semicolon not supported in this minishell */
void			free_exec_cmd(struct s_execcmd *ecmd);
void			free_redir_cmd(struct s_redircmd *rcmd);
void			free_pipe_cmd(struct s_pipecmd *pcmd);
void			free_back_cmd(struct s_backcmd *bcmd);
/* List command freeing removed - semicolon not supported in this minishell */
void			free_heredoc_cmd(struct s_heredoccmd *hcmd);
void			free_cmd(struct s_cmd *cmd);

/* Runner helper2 functions */
size_t			get_path_segment_len(char *curr, char **next);
int				build_full_path(char *full_path, char *curr,
					size_t len, const char *cmd);
void			reset_signals(void);
void			handle_exec_builtin(struct s_execcmd *ex, struct s_cmd *cmd,
					char ***env_copy);
int				open_redir_file_create(struct s_redircmd *rdir);

/* Runner helper3 functions */
int				open_redir_file_regular(struct s_redircmd *rdir);
/* List command functions removed - semicolon not supported in this minishell */

/* Runner helper4 functions */

/* Tokenize helper2 functions */
int				handle_basic_symbols(char **s_ptr, char *input_ptr);
int				handle_greater_than(char **s_ptr, char *input_ptr);
int				handle_less_than(char **s_ptr, char *input_ptr);
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

/* Env helper2 functions */
char			*handle_exit_status_var(void);
char			*search_env_variable(const char *name, size_t name_len,
					char **env_copy, char *temp);
int				handle_env_value_found(t_env_var_params params,
					char *env_value, size_t var_name_len, char **env_copy);
int				handle_env_value_not_found(t_env_var_params params,
					size_t var_name_len);

/* Main helper1 helper functions */
int				handle_eof(char *buf, size_t len);

/* Main helper4 functions */
void			sigint_handler_interactive(int signo);
void			sigquit_handler_interactive(int signo);
void			sigint_handler_noninteractive(int signo);
void			sigquit_handler_noninteractive(int signo);
void			setup_signals_interactive(void);

/* Main helper5 functions */
void			setup_signals_noninteractive(void);
int				validate_arguments(int argc, char **argv);
int				initialize_environment(char **envp, char ***environ_copy);
int				process_builtin_command(struct s_cmd *cmd, char *line,
					char ***environ_copy);
void			execute_external_command(struct s_cmd *cmd,
					char **environ_copy);

struct s_cmd	*pipecmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd	*backcmd(struct s_cmd *subcmd);
struct s_cmd	*redircmd(struct s_cmd *subcmd, char *file, char *efile,
					t_redir_params params);
struct s_cmd	*heredoccmd(struct s_cmd *subcmd, char *delimiter,
					char *content);
/* List command constructor removed - semicolon not supported */
struct s_cmd	*execcmd(void);
struct s_cmd	*parse_block(char **input_ptr, char *input_end,
					char **env_copy);
struct s_cmd	*parse_line(char **input_ptr, char *input_end,
					char **env_copy);

/* Parser helper functions */
char			*init_output_buffer(size_t len);
char			get_escape_char(char c);
void			handle_escape_sequence(const char *input, size_t *i,
					char *output, size_t *j);
char			*process_escaped(const char *input, size_t len);
int				get_redir_token(char **input_ptr, char *input_end, char **q,
					char **eq);
void			remove_redir_quotes(char **q, char **eq);
char			*process_filename(char *q, char *eq, char **env_copy);
struct s_cmd	*apply_input_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*apply_output_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*apply_append_redir(struct s_cmd *cmd, char *file);
struct s_cmd	*handle_redir_token(struct s_cmd *cmd, int tok, char *file);
struct s_cmd	*handle_heredoc_token(struct s_cmd *cmd, char *delimiter,
					char **env_copy, int is_quoted);
struct s_cmd	*init_exec_cmd(void);
int				get_exec_token(char **input_ptr, char *input_end, char **q,
					char **eq);
int				remove_exec_quotes(char **q, char **eq);
char			*process_argument(char *q, char *eq);
char			*process_argument_with_expansion(char *q, char *eq,
					char **env_copy, int quote_type);
char			*concatenate_quoted_strings(char **input_ptr, char *input_end,
					char **env_copy);
void			add_argument(struct s_execcmd *cmd, char *processed, int *argc);
void			finalize_exec_cmd(struct s_execcmd *cmd, int argc);
struct s_cmd	*process_arguments(struct s_cmd *ret,
					t_process_args_params params, char **env_copy);
struct s_cmd	*process_arguments_and_redirs(struct s_cmd *ret,
					t_process_args_params params, char **env_copy);

/* Built-in command handling */
int				is_builtin(char *cmd);
int				handle_builtin(char **argv, char ***env_copy);
char			*expand_variables(const char *str, size_t len,
					char **env_copy);
void			set_exit_status(int status);
int				get_exit_status(void);

/* Heredoc functions */
void			setup_heredoc_signals(void);
void			heredoc_sigint_handler(int signo);
char			*read_line_without_history(void);
char			*append_line_to_content(char *content, char *line);
int				check_delimiter_match(char *line, char *stripped_delimiter);
char			*read_heredoc_content(char *delimiter, char **env_copy,
					int is_quoted);
char			*append_line_to_content(char *content, char *line);

/* Builtin helper functions */
int				cd_to_home(char **env_copy);
int				cd_to_path(char *path);
int				parse_export_arg(char *arg, char **name, char **value);
void			remove_quotes(char **value);
void			format_export_output(char *env_var);
int				set_environment_var(char *name, char *value,
					char ***env_copy);
void			print_sorted_env_vars(char **env_copy);
void			update_pwd_variables(char *old_pwd, char *new_pwd,
					char ***env_copy);

/* Builtin command functions */
int				builtin_echo(char **argv);
int				builtin_cd(char **argv, char ***env_copy);
int				builtin_pwd(char **argv);
int				builtin_exit(char **argv);
int				builtin_env(char **argv, char ***env_copy);
char			*create_old_pwd_var(char *old_pwd);
char			*create_new_pwd_var(char *new_pwd);
int				count_env_vars_pwd(char ***env_copy);
char			**allocate_new_environ_pwd(char ***env_copy, int count);
void			copy_and_update_env_vars(char ***env_copy, char **new_environ,
					char *old_pwd_var, char *new_pwd_var);
void			finalize_new_environ_pwd(char **new_environ, int count,
					char *old_pwd_var, char *new_pwd_var);

/* Runner helper functions */
char			*check_absolute_path(const char *cmd);
size_t			get_path_segment_len(char *curr, char **next);
int				build_full_path(char *full_path, char *curr, size_t len,
					const char *cmd);
char			*search_in_paths(char *path, const char *cmd);
void			reset_signals(void);
void			expand_exec_args(struct s_execcmd *ex, char **env_copy);
void			handle_exec_builtin(struct s_execcmd *ex, struct s_cmd *cmd,
					char ***env_copy);
void			execute_external_cmd(struct s_execcmd *ex, char **env_copy);
int				open_redir_file_create(struct s_redircmd *rdir);
int				open_redir_file_regular(struct s_redircmd *rdir);
/* List command functions removed - semicolon not supported in this minishell */

/* Utils3 functions - Custom implementations of forbidden functions */
int				ft_strcmp(const char *s1, const char *s2);
int				ft_snprintf(char *str, size_t size, const char *format, ...);
int				ft_vsnprintf(char *str, size_t size, const char *format,
					va_list args);
void			*ft_realloc(void *ptr, size_t size);
char			*ft_strncpy(char *dest, const char *src, size_t n);
char			*ft_strcpy(char *dest, const char *src);
int				ft_fflush_stdout(void);
int				ft_fprintf_stderr(const char *format, ...);
int				ft_setenv(const char *name, const char *value, int overwrite,
					char ***env_copy);
int				ft_unsetenv(const char *name, char ***env_copy);
void			print_missing_paren(const char *paren);
void			print_missing_file_name(void);
void			print_malloc_failed(void);
void			print_syntax_error(void);
void			print_exit_too_many_args(void);
void			print_command_not_found(const char *cmd);
void			print_too_many_args(void);
void			print_unknown_command_type(int type);
void			print_heredoc_eof_warning(const char *delimiter);
void			print_cd_home_not_set(void);
void			print_cd_getcwd_failed(void);
void			print_cd_too_many_args(void);
void			print_cd_error(const char *path, const char *error);
void			print_export_invalid_identifier(const char *arg);
void			print_export_error(const char *error);
void			print_path_too_long(const char *curr, const char *cmd);
void			print_open_failed(const char *file, const char *error);
void			print_dup2_failed(const char *error);
void			print_exit_numeric_arg(const char *arg);

/* Memory management functions */
void			update_shlvl_value(char ***env_copy, int i, int shlvl_num);
void			create_new_shlvl(char ***env_copy);
void			increment_shlvl(char ***env_copy);
int				count_envp_vars(char **envp);
void			cleanup_on_failure(char **new_environ, int i);
int				copy_env_vars(char **envp, char **new_environ);
char			**copy_environ(char **envp);
void			free_environ_copy(char **environ_copy);
void			clean_exit(int status);
# ifdef DEBUG

void			check_leaks(void);
# endif

/* Command execution functions */
void			runcmd(struct s_cmd *cmd, char **env_copy);
void			run_exec_cmd(struct s_cmd *cmd, char **env_copy);
void			run_redir_cmd(struct s_cmd *cmd, char **env_copy);
void			run_pipe_cmd(struct s_cmd *cmd, char **env_copy);
void			run_back_cmd(struct s_cmd *cmd, char **env_copy);
void			run_heredoc_cmd(struct s_cmd *cmd, char **env_copy);
void			run_list_cmd(struct s_cmd *cmd);
void			restore_stdin(int saved_stdin);
void			restore_stdout(int saved_stdout);
void			handle_redir_error(struct s_redircmd *rdir, int saved_stdin,
					int saved_stdout);
int				setup_redirection(struct s_redircmd *rdir, int *saved_stdin,
					int *saved_stdout);
int				open_redirection_file(struct s_redircmd *rdir, int saved_stdin,
					int saved_stdout);
void			restore_redirection(struct s_redircmd *rdir, int saved_stdin,
					int saved_stdout);
void			setup_pipe_left(int *p, struct s_pipecmd *pipecmd,
					char **env_copy);
void			setup_pipe_right(int *p, struct s_pipecmd *pipecmd,
					char **env_copy);
char			*find_command(const char *cmd, char **env_copy);
void			execute_external_cmd(struct s_execcmd *ex, char **env_copy);
void			handle_child_process(char *cmd_path, char **av,
					char **env_copy);
void			handle_parent_process(pid_t pid, char *cmd_path, int *status);
void			handle_exit_status(int status);
int				setup_pipe(int *p);
int				create_first_child(int *p, struct s_pipecmd *pipecmd,
					char **env_copy);
int				create_second_child(int *p, struct s_pipecmd *pipecmd,
					char **env_copy, pid_t pid1);
void			wait_for_children(pid_t pid1, pid_t pid2, int *status1,
					int *status2);
void			handle_pipe_status(int status2);
void			handle_background_child(struct s_backcmd *backcmd,
					char **env_copy);
void			handle_background_status(int status);
int				setup_heredoc_pipe(int *pipe_fd, int *saved_stdin);
int				create_heredoc_child(int *pipe_fd, struct s_heredoccmd *hcmd);
int				check_heredoc_child_status(int pid, int *pipe_fd,
					int saved_stdin);
int				setup_stdin_redirection(int *pipe_fd, int saved_stdin);
int				handle_d_format(char *str, int *i, int size, va_list args);
int				handle_s_format(char *str, int *i, int size, va_list args);
int				handle_c_format(char *str, int *i, int size, va_list args);
int				handle_format_specifier(t_format_params params);
int				process_format_loop(t_format_params params);
int				update_existing_var(char ***env_copy, int i, const char *name,
					const char *value);
int				count_env_vars(char ***env_copy);
int				allocate_new_environ(char ***env_copy, char **new_var,
					char ***new_environ);
void			copy_existing_env_vars(char ***env_copy, char **new_environ,
					int count);
void			finalize_new_environ(char **new_environ, int count,
					char *new_var, char ***env_copy);
int				add_new_var(char ***env_copy, const char *name,
					const char *value);
int				count_total_vars(char ***env_copy);
void			copy_vars_except_index(char ***env_copy, char **new_environ,
					int var_index);
int				remove_var_from_env(char ***env_copy, int var_index);
int				find_var_index(char ***env_copy, const char *name,
					int name_len);
int				check_n_flag(char *arg);
int				process_n_flags(char **argv, int *i);
void			print_arguments(char **argv, int start_index);
char			*get_current_directory(void);
int				handle_cd_no_args(char *old_pwd, char ***env_copy);
int				handle_cd_with_args(char **argv, char *old_pwd);
int				update_pwd_and_cleanup(char *old_pwd, char ***env_copy);
int				validate_numeric_arg(char *arg);
int				check_too_many_args(char **argv);

#endif