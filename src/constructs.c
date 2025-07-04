#include "minishell.h"
#include <stdlib.h>
#include <string.h>

struct s_cmd* execcmd(void)
{
    struct s_execcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC;
    return (struct s_cmd *)cmd;
}

struct s_cmd* redircmd(struct s_cmd *subcmd, char *file, char *efile, int mode, int fd)
{
    struct s_redircmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = REDIR;
    cmd->cmd = subcmd;
    cmd->file = file;
    cmd->efile = efile;
    cmd->mode = mode;
    cmd->fd = fd;
    return (struct s_cmd *)cmd;
}

struct s_cmd* pipecmd(struct s_cmd *left, struct s_cmd *right)
{
    struct s_pipecmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = PIPE;
    cmd->left = left;
    cmd->right = right;
    return (struct s_cmd *)cmd;
}

struct s_cmd* listcmd(struct s_cmd *left, struct s_cmd *right)
{
    struct s_listcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = LIST;
    cmd->left = left;
    cmd->right = right;
    return (struct s_cmd *)cmd;
}

struct s_cmd* backcmd(struct s_cmd *subcmd)
{
    struct s_backcmd *cmd;
    
    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = BACK;
    cmd->cmd = subcmd;
    return (struct s_cmd *)cmd;
}
    