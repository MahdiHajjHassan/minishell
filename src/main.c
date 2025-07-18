#include "minishell.h"
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/* Initialize global signal structure */
t_sig g_sig = {0, 0, 0, 0};

/*
 * display_prompt - Show shell prompt
 */
static void display_prompt(void)
{
    char cwd[1024];
    
    if (isatty(STDIN_FILENO)) {
        if (get_cwd(cwd, sizeof(cwd))) {
            printf("%s> ", cwd);
            fflush(stdout);
        }
    }
}

/*
 * Signal handler for SIGINT (Ctrl-C)
 */
static void sigint_handler(int signo)
{
    (void)signo;
    if (g_sig.pid == 0) {
        write(STDERR_FILENO, "\b\b  \b\b", 6);  // Erase ^C characters
        write(STDERR_FILENO, "\n", 1);
        display_prompt();  // Redisplay prompt after Ctrl-C
        g_sig.exit_status = 1;
    } else {
        write(STDERR_FILENO, "\n", 1);
        g_sig.exit_status = 130;
    }
    g_sig.sigint = 1;
}

/*
 * Signal handler for SIGQUIT (Ctrl-\)
 */
static void sigquit_handler(int signo)
{
    (void)signo;
    if (g_sig.pid != 0) {
        write(STDERR_FILENO, "Quit: 3\n", 8);
        g_sig.exit_status = 131;
        g_sig.sigquit = 1;
    } else {
        write(STDERR_FILENO, "\b\b  \b\b", 6);
    }
}

/*
 * setup_signals - Configure signal handling for the shell
 */
static void setup_signals(void)
{
    struct sigaction sa_int, sa_quit;
    
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = sigquit_handler;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
 * readline_helper - Read a line of input from the user with proper Ctrl+D handling
 */
char *readline_helper(void)
{
    char    *buf = NULL;
    size_t  capacity = 128;
    size_t  len = 0;
    int     c;
    
    buf = malloc(capacity);
    if (!buf)
        return (NULL);
    
    display_prompt();
    
    while (1)
    {
        c = getchar();
        
        if (c == EOF)
        {
            // Ctrl+D pressed
            if (len == 0)
            {
                // Empty prompt - quit the shell
                if (isatty(STDIN_FILENO))
                    printf("exit\n");
                free(buf);
                exit(g_last_exit_status);
            }
            // Non-empty prompt - clear EOF state and return current buffer
            clearerr(stdin);
            buf[len] = '\0';
            
        }
        
        if (c == '\n')
        {
            // End of line
            buf[len] = '\0';
            return (buf);
        }
        
        // Handle backspace
        if (c == 127 || c == 8) // DEL or BS
        {
            if (len > 0)
            {
                len--;
                // Visual feedback for backspace
                if (isatty(STDIN_FILENO))
                {
                    printf("\b \b");
                    fflush(stdout);
                }
            }
            continue;
        }
        
        // Regular character
        if (len + 1 >= capacity)
        {
            capacity *= 2;
            char *new_buf = realloc(buf, capacity);
            if (!new_buf)
            {
                free(buf);
                return (NULL);
            }
            buf = new_buf;
        }
        
        buf[len++] = c;
    }
}

/*
 * main - Main shell loop
 */
int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    
    setup_signals();
    char *line;
    struct s_cmd *cmd;
    struct s_execcmd *ecmd;
    int status;

    while (1)
    {
        g_sig.sigint = 0;
        g_sig.sigquit = 0;
        g_sig.pid = 0;
        
        line = readline_helper();
        if (!line) {
            display_prompt();
            continue;
        }
        
        cmd = tokenize(line);
        if (!cmd) {
            free(line);
            continue;
        }

        if (cmd->type == EXEC) {
            ecmd = (struct s_execcmd *)cmd;
            if (ecmd->av[0] && is_builtin(ecmd->av[0])) {
                status = handle_builtin(ecmd->av);
                set_exit_status(status);
                free(line);
                continue;
            }
        }

        g_sig.pid = forkk();
        if (g_sig.pid == 0) {
            runcmd(cmd);
            exit(0);
        }
        wait(&status);
        if (WIFSIGNALED(status)) {
            set_exit_status(128 + WTERMSIG(status));
        } else {
            set_exit_status(WEXITSTATUS(status));
        }
        free(line);
    }
    return (g_last_exit_status);
}
