#ifndef MINISHELL_H
# define MINISHELL_H

# include <ctype.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* Libft helper functions */
size_t			ft_strlen(const char *s);
char			*ft_strchr(const char *s, int c);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strdup(const char *s);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
size_t			ft_strlcat(char *dst, const char *src, size_t size);
int				ft_atoi(const char *nptr);
char			*ft_itoa(int n);
char			*ft_strnstr(const char *haystack,
					const char *needle, size_t len);

/* Signal handling structure */
typedef struct s_sig
{
	volatile sig_atomic_t	sigint;
	volatile sig_atomic_t	sigquit;
	volatile pid_t			pid;
	volatile int			exit_status;
}				t_sig;

extern t_sig				g_sig;

/* Command Types */
# define EXEC 1
# define REDIR 2
# define PIPE 3
# define LIST 4
# define BACK 5
# define HEREDOC 6

# define MAXARGS 100

extern int					g_last_exit_status;

typedef struct s_cmd
{
	int						type;
}				t_cmd;

typedef struct s_execcmd
{
	int						type;
	char					*av[MAXARGS];
	char					*eav[MAXARGS];
}				t_execcmd;

typedef struct s_redircmd
{
	int						type;
	struct s_cmd			*cmd;
	char					*file;
	char					*efile;
	int						mode;
	int						fd;
}				t_redircmd;

typedef struct s_pipecmd
{
	int						type;
	struct s_cmd			*left;
	struct s_cmd			*right;
}				t_pipecmd;

typedef struct s_listcmd
{
	int						type;
	struct s_cmd			*left;
	struct s_cmd			*right;
}				t_listcmd;

typedef struct s_backcmd
{
	int						type;
	struct s_cmd			*cmd;
}				t_backcmd;

typedef struct s_heredoccmd
{
	int						type;
	struct s_cmd			*cmd;
	char					*delimiter;
	int						fd;
}				t_heredoccmd;

/* Basic shell functions */
char			*readline_helper(void);
void			*get_cwd(char *buf, size_t size);
void			wtf(void);
int				forkk(void);
void			runcmd(struct s_cmd *cmd);
struct s_cmd	*tokenize(const char *line);
struct s_cmd	*nulterm(struct s_cmd *cmd);

/* Parser and command constructor functions */
struct s_cmd	*parseexec(char **input_ptr, char *input_end);
int				peek(char **input_ptr, char *input_end, char *toks);
int				gettoken(char **input_ptr, char *input_end,
					char **token_start, char **token_end);
struct s_cmd	*pipecmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd	*backcmd(struct s_cmd *subcmd);
struct s_cmd	*listcmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd	*redircmd(struct s_cmd *subcmd, char *file,
					char *efile, int mode, int fd);
struct s_cmd	*execcmd(void);
struct s_cmd	*parse_redirs(struct s_cmd *cmd,
					char **input_ptr, char *input_end);
struct s_cmd	*parse_block(char **input_ptr, char *input_end);
struct s_cmd	*parse_line(char **input_ptr, char *input_end);
struct s_cmd	*heredoccmd(struct s_cmd *subcmd, char *delimiter, int fd);
char			*process_escaped(const char *input, size_t len);

/* Built-in command handling */
int				is_builtin(char *cmd);
int				handle_builtin(char **argv);
char			*expand_variables(const char *str, size_t len);
void			set_exit_status(int status);

/* Builtin functions */
int				builtin_echo(char **argv);
int				builtin_cd(char **argv);
int				builtin_pwd(char **argv);
int				builtin_export(char **argv);
int				builtin_unset(char **argv);
int				builtin_env(char **argv);
int				builtin_exit(char **argv);

/* Internal command handlers */
char			*find_command(const char *cmd);
void			handle_exec_cmd(struct s_cmd *cmd);
void			handle_redir_cmd(struct s_cmd *cmd);
void			handle_list_cmd(struct s_cmd *cmd);
void			handle_pipe_cmd(struct s_cmd *cmd);
void			handle_back_cmd(struct s_cmd *cmd);
void			handle_heredoc_cmd(struct s_cmd *cmd);

#endif