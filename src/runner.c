#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *find_command(const char *cmd)
{
    char *path = getenv("PATH");
    char *curr;
    char *next;
    char full_path[1024];
    size_t len;

    if (!path)
        return NULL;

    // Handle absolute or relative paths
    if (cmd[0] == '/' || cmd[0] == '.') {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        return NULL;
    }

    curr = path;
    while (curr && *curr)
    {
        next = strchr(curr, ':');
        len = next ? (size_t)(next - curr) : strlen(curr);
        
        if (len + strlen(cmd) + 2 > sizeof(full_path))
            return NULL;
            
        strncpy(full_path, curr, len);
        full_path[len] = '/';
        strcpy(full_path + len + 1, cmd);
        
        if (access(full_path, X_OK) == 0)
            return strdup(full_path);
            
        if (!next)
            break;
        curr = next + 1;
    }
    return NULL;
}

void runcmd(struct s_cmd *cmd)
{
    int p[2];
    struct s_backcmd *back;
    struct s_execcmd *ex;
    struct s_listcmd *list;
    struct s_pipecmd *pipecmd;
    struct s_redircmd *rdir;
    char *cmd_path;
    extern char **environ;  // Get the current environment
    
    if (cmd == 0)
        exit(0);
    if (cmd->type == EXEC)
    {
        ex = (struct s_execcmd *)cmd;
        if (ex->av[0] == 0)
            exit(0);
        fprintf(stderr, "DEBUG: about to exec '%s'\n", ex->av[0]);
        
        // Check for builtin commands first
        if (is_builtin(ex->av[0])) {
            exit(handle_builtin(ex->av));
        }
        
        cmd_path = find_command(ex->av[0]);
        if (cmd_path) {
            execve(cmd_path, ex->av, environ);
            free(cmd_path);
        }
        fprintf(stderr, "exec %s espisiallyfailed\n", ex->av[0]);
        exit(0);
    }
    if (cmd->type == REDIR)
    {
        rdir = (struct s_redircmd *)cmd;
        close(rdir->fd);
        int fd = open(rdir->file, rdir->mode, 0644);
        if (fd < 0)
        {
            fprintf(stderr, "redirect: %s: open failed\n", rdir->file);
            exit(1);
        }
        if (fd != rdir->fd) {
            dup2(fd, rdir->fd);
            close(fd);
        }
        runcmd(rdir->cmd);
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
        if (forkk() == 0) {
            if (forkk() == 0) {
                runcmd(back->cmd);
            }
            exit(0);
        }
        wait(0); // wait first baby only cuz life is not fair
        return;
    }
}