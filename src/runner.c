#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> // Required for errno

/*
 * find_command - Locate executable in PATH
 * 
 * Searches for a command in the directories specified by PATH.
 * Handles both absolute/relative paths and commands that need PATH lookup.
 * 
 * @cmd: Command name to find
 * 
 * Returns:
 *   - Allocated string with full path on success
 *   - NULL if command not found or not executable
 */
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
        
        if (len + strlen(cmd) + 2 > sizeof(full_path)) {
            fprintf(stderr, "Path too long: %.*s/%s\n", (int)len, curr, cmd);
            return NULL;
        }
            
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

/*
 * runcmd - Execute a command structure
 * 
 * This is the core execution function that handles all command types:
 * - EXEC: Simple command execution (ls -l)
 * - REDIR: Input/output redirection (cmd > file)
 * - LIST: Sequential commands (cmd1 ; cmd2)
 * - PIPE: Pipeline between commands (cmd1 | cmd2)
 * - BACK: Background execution (cmd &)
 * 
 * @cmd: Command structure to execute
 */
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

    switch (cmd->type) {
        case EXEC:
            ex = (struct s_execcmd *)cmd;
            if (ex->av[0] == 0)
                exit(0);
            fprintf(stderr, "DEBUG: executing command '%s'\n", ex->av[0]);
            
            // Check for builtin commands first
            if (is_builtin(ex->av[0])) {
                exit(handle_builtin(ex->av));
            }
            
            // Find and execute external command
            cmd_path = find_command(ex->av[0]);
            if (cmd_path) {
                fprintf(stderr, "DEBUG: found command at '%s'\n", cmd_path);
                execve(cmd_path, ex->av, environ);
                perror("execve failed");  // Only reached if execve fails
                free(cmd_path);
            } else {
                fprintf(stderr, "command not found: %s\n", ex->av[0]);
            }
            exit(127);  // Command not found exit code

        case REDIR:
            rdir = (struct s_redircmd *)cmd;
            close(rdir->fd);
            if ((rdir->mode & O_CREAT) && (rdir->mode & (O_WRONLY | O_RDWR))) {
                int fd = open(rdir->file, rdir->mode, 0644);
                if (fd < 0) {
                    fprintf(stderr, "open failed: %s: %s\n", rdir->file, strerror(errno));
                    exit(1);
                }
                if (fd != rdir->fd) {
                    dup2(fd, rdir->fd);
                    close(fd);
                }
            } else {
                if (open(rdir->file, rdir->mode) < 0) {
                    fprintf(stderr, "open failed: %s: %s\n", rdir->file, strerror(errno));
                    exit(1);
                }
            }
            runcmd(rdir->cmd);
            break;

        case LIST:
            list = (struct s_listcmd *)cmd;
            if (forkk() == 0)
                runcmd(list->left);
            wait(0);
            runcmd(list->right);
            break;

        case PIPE:
            pipecmd = (struct s_pipecmd *)cmd;
            if (pipe(p) < 0) {
                perror("pipe failed");
                wtf();
            }
            
            // Left side of pipe
            if (forkk() == 0) {
                close(1);
                dup(p[1]);
                close(p[0]);
                close(p[1]);
                runcmd(pipecmd->left);
            }
            
            // Right side of pipe
            if (forkk() == 0) {
                close(0);
                dup(p[0]);
                close(p[0]);
                close(p[1]);
                runcmd(pipecmd->right);
            }
            
            // Parent closes pipe and waits
            close(p[0]);
            close(p[1]);
            wait(0);
            wait(0);
            break;

        case BACK:
            back = (struct s_backcmd *)cmd;
            if (forkk() == 0) {
                if (forkk() == 0) {
                    // Double fork to fully detach
                    runcmd(back->cmd);
                }
                exit(0);
            }
            wait(0);  // Wait for first child only
            break;

        default:
            fprintf(stderr, "unknown command type: %d\n", cmd->type);
            exit(1);
    }
    exit(0);
}