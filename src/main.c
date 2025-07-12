#include "minishell.h"

/*
 * readline_helper - Read a line of input from the user with prompt
 * 
 * Displays the current working directory as prompt and reads user input.
 * Handles EOF and error conditions appropriately.
 * 
 * Returns:
 *   - Pointer to the line read (caller must free)
 *   - NULL on EOF or error
 */
char *readline_helper(void)
{
    char    *buf;
    size_t  size = 0;
    char    cwd[1024];

    buf = NULL;
    get_cwd(cwd, sizeof(cwd));
    // Only show prompt if we're running interactively
    if (isatty(fileno(stdin)))
        printf("%s> ", cwd);
    if (getline(&buf, &size, stdin) == -1)
    {
        if (feof(stdin))
        {
            printf("[EOF]\n");
            free(buf);
            return NULL;
        }
        else
        {
            printf("Getline is dead\n");
            free(buf);
            return NULL;
        }
    }
    return (buf);
}

/*
 * main - Main shell loop
 * 
 * This is the core of the shell. It:
 * 1. Reads commands from user input
 * 2. Parses them into command structures
 * 3. Handles built-in commands directly
 * 4. Forks and executes external commands
 * 5. Maintains exit status of commands
 * 
 * The shell continues running until EOF (Ctrl+D) is received.
 */
int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    char *line;
    struct s_cmd *cmd;
    struct s_execcmd *ecmd;
    int status;

    while (1)
    {
        // Read and parse input
        line = readline_helper();
        if (!line)
            break;  // EOF received
        cmd = tokenize(line);

        // Check if it's a built-in command
        if (cmd->type == EXEC) {
            ecmd = (struct s_execcmd *)cmd;
            if (ecmd->av[0] && is_builtin(ecmd->av[0])) {
                status = handle_builtin(ecmd->av);
                set_exit_status(status);
                free(line);
                continue;
            }
        }

        // Fork and execute external commands
        if (forkk() == 0) {
            runcmd(cmd);
            exit(0);  // Child process exits here
        }
        // Parent waits for command to complete
        wait(&status);
        set_exit_status(status);
        free(line);
    }
    return (g_last_exit_status);
}
