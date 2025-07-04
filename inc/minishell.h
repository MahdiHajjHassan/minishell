#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"

#define EXEC 1
#define REDIR 2
#define PIPE 3
#define LIST 4
#define BACK 5
#define MAXARGS 10

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


#endif