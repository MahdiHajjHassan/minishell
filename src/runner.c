#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void runcmd(struct s_cmd *cmd)
{
    int p[2];
    struct s_backcmd *back;
    struct s_execcmd *ex;
    struct s_listcmd *list;
    struct s_pipecmd *pipecmd;
    struct s_redircmd *rdir;
    
    if (cmd == 0)
        exit(0);
    if (cmd->type == EXEC)
    {
        ex = (struct s_execcmd *)cmd;
        if (ex->av[0] == 0)
            exit(0);
        execvp(ex->av[0], ex->av);
        fprintf(stderr, "exec %s: not found\n", ex->av[0]);
        exit(0);
    }
    if (cmd->type == REDIR)
    {
        rdir = (struct s_redircmd *)cmd;
        close(rdir->fd);
        if (open(rdir->file, rdir->mode) < 0)
        {
            fprintf(stderr, "redirect: %s: open: %s\n", rdir->file, rdir->file);
            exit(1);
        }
        runcmd(rdir->cmd);
        close(rdir->fd);
        return;
    }
    if (cmd->type == LIST)
    {
        list = (struct s_listcmd *)cmd;
        if (forkk() == 0)
            runcmd(list->left);
        wait(0);
        runcmd(list->right);
        return;
    }
    if (cmd->type == PIPE)
    {
        pipecmd = (struct s_pipecmd *)cmd;
        if (pipe(p) < 0)
            wtf();
        if (forkk() == 0)
        {
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            runcmd(pipecmd->left);
        }
        if (forkk() == 0)
        {
            close(0);
            dup(p[0]);
            close(p[0]);
            close(p[1]);
            runcmd(pipecmd->right);
        }
        close(p[0]);
        close(p[1]);
        wait(0);
        wait(0);
        return;
    }
    if (cmd->type == BACK)
    {
        back = (struct s_backcmd *)cmd;
        if (forkk() == 0)
            runcmd(back->cmd);
        return;
    }
}