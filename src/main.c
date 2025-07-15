#include "minishell.h"
#include <signal.h>
#include <errno.h>

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
 * readline_helper - Read a line of input from the user
 */
char *readline_helper(void)
{
    char    *buf;
    size_t  size;
    ssize_t read;

    buf = NULL;
    size = 0;
    display_prompt();
    
    read = getline(&buf, &size, stdin);
    if (read == -1) {
        if (feof(stdin)) {
            if (isatty(STDIN_FILENO))
                printf("exit\n");
            free(buf);
            exit(0);
        }
        free(buf);
        return NULL;
    }
    return buf;
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
