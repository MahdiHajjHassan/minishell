#include "minishell.h"
#include <stdlib.h>
#include <string.h>

/*
 * execcmd - Create a new execution command structure
 * 
 * Allocates and initializes a structure for simple command execution.
 * Used for commands like: ls -l /home
 * 
 * Returns:
 *   - Pointer to new command structure
 *   - All fields initialized to 0/NULL
 */
struct s_cmd* execcmd(void)
{
    struct s_execcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC;
    return (struct s_cmd *)cmd;
}

/*
 * redircmd - Create a new redirection command structure
 * 
 * Sets up command redirection (input/output).
 * Used for commands like: ls > file.txt
 * 
 * @subcmd: Command to be redirected
 * @file: Target file name
 * @efile: Pointer to end of file name
 * @mode: File open mode (O_RDONLY, O_WRONLY, etc.)
 * @fd: File descriptor to redirect (0=stdin, 1=stdout)
 * 
 * Returns: New redirection command structure
 */
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

/*
 * pipecmd - Create a new pipe command structure
 * 
 * Connects two commands with a pipe.
 * Used for commands like: ls | grep txt
 * 
 * @left: Command before the pipe
 * @right: Command after the pipe
 * 
 * Returns: New pipe command structure
 */
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

/*
 * listcmd - Create a new command list structure
 * 
 * Connects two commands to be executed sequentially.
 * Used for commands like: echo hello ; ls
 * 
 * @left: First command to execute
 * @right: Second command to execute
 * 
 * Returns: New list command structure
 */
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

/*
 * backcmd - Create a new background command structure
 * 
 * Sets up a command to run in the background.
 * Used for commands like: sleep 100 &
 * 
 * @subcmd: Command to run in background
 * 
 * Returns: New background command structure
 */
struct s_cmd* backcmd(struct s_cmd *subcmd)
{
    struct s_backcmd *cmd;
    
    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = BACK;
    cmd->cmd = subcmd;
    return (struct s_cmd *)cmd;
}
    