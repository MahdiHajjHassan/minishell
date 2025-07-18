#include "minishell.h"

/*
 * get_cwd - Get current working directory safely
 * 
 * Wrapper around getcwd that adds error handling.
 * 
 * @buf: Buffer to store path
 * @size: Size of buffer
 * 
 * Returns:
 *   - Pointer to buf on success
 *   - NULL on error with error message printed
 */
void *get_cwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
	{
		perror("getcwd is dead");
		return NULL;
	}
	return buf;
}

/*
 * forkk - Fork wrapper with error handling
 * 
 * Creates a new process and handles fork failures.
 * 
 * Returns:
 *   - Process ID of child in parent
 *   - 0 in child
 *   - Exits program on fork failure
 */
int forkk(void)
{
	int pid;

	pid = fork();
	if (pid == -1)
		wtf();
	return pid;
}

/*
 * wtf - Fatal error handler
 * 
 * Prints error message and exits program.
 * Used for unrecoverable errors.
 */
void wtf(void)
{
	printf("wtf\n");
	exit(1);
}

/*
 * nulterm - Ensure proper string termination in command tree
 * 
 * Recursively traverses command tree and ensures all strings
 * are properly null-terminated. This is necessary because we
 * use end pointers (eav) instead of copying strings.
 * 
 * @cmd: Root of command tree
 * 
 * Returns: Same command tree with null-terminated strings
 */
struct s_cmd* nulterm(struct s_cmd *cmd)
{
    int i = 0;
    struct s_backcmd *bcmd;
    struct s_execcmd *ecmd;
    struct s_listcmd *lcmd;
    struct s_pipecmd *pcmd;
    struct s_redircmd *rcmd;

    if (cmd == 0)
        return 0;
    switch (cmd->type)
    {
        case EXEC:
            ecmd = (struct s_execcmd *)cmd;
            // Null terminate each argument string
            for (i = 0; ecmd->av[i]; i++)
                *ecmd->eav[i] = 0;
            break;
        case REDIR:
            rcmd = (struct s_redircmd *)cmd;
            nulterm(rcmd->cmd);
            *rcmd->efile = 0;  // Null terminate filename
            break;
        case PIPE:
            pcmd = (struct s_pipecmd *)cmd;
            // Recursively handle both sides of pipe
            nulterm(pcmd->left);
            nulterm(pcmd->right);
            break;
        case LIST:
            lcmd = (struct s_listcmd *)cmd;
            // Recursively handle both commands in list
            nulterm(lcmd->left);
            nulterm(lcmd->right);
            break;
        case BACK:
            bcmd = (struct s_backcmd *)cmd;
            // Recursively handle background command
            nulterm(bcmd->cmd);
            break;
        case HEREDOC:
            {
                struct s_heredoccmd *hcmd = (struct s_heredoccmd *)cmd;
                // Recursively handle the command
                nulterm(hcmd->cmd);
            }
            break;
    }
    return cmd;
}
