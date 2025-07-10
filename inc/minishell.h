#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"

#define EXEC 1
#define REDIR 2
#define PIPE 3
#define LIST 4
#define BACK 5

#define MAXARGS 10

// Add global variable for last exit status
extern int g_last_exit_status;

typedef struct s_cmd
{
	int type;
}	t_cmd;

typedef struct s_execcmd
{
	int type;
	char *av[MAXARGS];
	char *ev[MAXARGS];
}	t_execcmd;

typedef struct s_redircmd
{
	int type;
	struct s_cmd *cmd;
	char *file;
	char *efile;
	int mode;
	int fd;
}	t_redircmd;

typedef struct s_pipecmd
{
	int type;
	struct s_cmd *left;
	struct s_cmd *right;
}	t_pipecmd;

typedef struct s_listcmd
{
	int type;
	struct s_cmd *left;
	struct s_cmd *right;
}	t_listcmd;

typedef struct s_backcmd
{
	int type;
	struct s_cmd *cmd;
}	t_backcmd;






char *readline_helper(void);
void *get_cwd(char *buf, size_t size);
void wtf(void);
int forkk(void);
void runcmd(struct s_cmd *cmd);
struct s_cmd *tokenize(const char *line);
struct s_cmd* nulterm(struct s_cmd *cmd);

// Parser and command constructors
struct s_cmd *parseexec(char **input_ptr, char *input_end);
int peek(char **input_ptr, char *input_end, char *toks);
int gettoken(char **input_ptr, char *input_end, char **token_start, char **token_end);
struct s_cmd *pipecmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd *backcmd(struct s_cmd *subcmd);
struct s_cmd *listcmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd *redircmd(struct s_cmd *subcmd, char *file, char *efile, int mode, int fd);
struct s_cmd *execcmd(void);
struct s_cmd *parse_redirs(struct s_cmd *cmd, char **input_ptr, char *input_end);
struct s_cmd *parse_block(char **input_ptr, char *input_end);
struct s_cmd *parse_line(char **input_ptr, char *input_end);

// Add after other function prototypes
int is_builtin(char *cmd);
int handle_builtin(char **argv);
char *expand_variables(const char *str, size_t len);
void set_exit_status(int status);

#endif