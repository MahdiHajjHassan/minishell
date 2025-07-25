#include "minishell.h"

/* Initialize global signal structure */
t_sig			g_sig = {0, 0, 0, 0};

/*
 * display_prompt - Show shell prompt
 */
static void	display_prompt(void)
{
	char	cwd[1024];

	if (isatty(STDIN_FILENO))
	{
		if (get_cwd(cwd, sizeof(cwd)))
		{
			printf("%s> ", cwd);
		}
	}
}

/*
 * Signal handler for SIGINT (Ctrl-C)
 */
static void	sigint_handler(int signo)
{
	(void)signo;
	if (g_sig.pid == 0)
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
		write(STDERR_FILENO, "\n", 1);
		display_prompt();
		g_sig.exit_status = 1;
	}
	else
	{
		write(STDERR_FILENO, "\n", 1);
		g_sig.exit_status = 130;
	}
	g_sig.sigint = 1;
}

/*
 * Signal handler for SIGQUIT (Ctrl-\)
 */
static void	sigquit_handler(int signo)
{
	(void)signo;
	if (g_sig.pid != 0)
	{
		write(STDERR_FILENO, "Quit: 3\n", 8);
		g_sig.exit_status = 131;
		g_sig.sigquit = 1;
	}
	else
	{
		write(STDERR_FILENO, "\b\b  \b\b", 6);
	}
}

/*
 * setup_signals - Configure signal handling for the shell
 */
static void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

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
 * handle_eof - Handle EOF (Ctrl+D) in input
 *
 * @len: Current buffer length
 * @buf: Input buffer
 *
 * Returns: 1 if should quit, 0 if should continue
 */
static int	handle_eof(size_t len, char *buf)
{
	if (len == 0)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		free(buf);
		exit(g_last_exit_status);
	}
	clearerr(stdin);
	return (0);
}

/*
 * handle_backspace - Handle backspace key
 *
 * @len: Current buffer length
 *
 * Returns: Updated length
 */
static size_t	handle_backspace(size_t len)
{
	if (len > 0)
	{
		len--;
		if (isatty(STDIN_FILENO))
		{
			printf("\b \b");
		}
	}
	return (len);
}

/*
 * expand_buffer - Expand input buffer if needed
 *
 * @buf: Current buffer
 * @capacity: Current capacity
 * @len: Current length
 *
 * Returns: New buffer or NULL on failure
 */
static char	*expand_buffer(char *buf, size_t *capacity, size_t len)
{
	char	*new_buf;

	if (len + 1 >= *capacity)
	{
		*capacity *= 2;
		new_buf = realloc(buf, *capacity);
		if (!new_buf)
		{
			free(buf);
			return (NULL);
		}
		buf = new_buf;
	}
	return (buf);
}

/*
 * readline_helper
	- Read a line of input from the user with proper Ctrl+D handling
 */
char	*readline_helper(void)
{
	char	*buf;
	size_t	capacity;
	size_t	len;
	int		c;

	buf = NULL;
	capacity = 128;
	len = 0;
	buf = malloc(capacity);
	if (!buf)
		return (NULL);
	display_prompt();
	while (1)
	{
		c = getchar();
		if (c == EOF)
		{
			if (handle_eof(len, buf))
				return (NULL);
			buf[len] = '\0';
			return (buf);
		}
		if (c == '\n')
		{
			buf[len] = '\0';
			return (buf);
		}
		if (c == 127 || c == 8)
		{
			len = handle_backspace(len);
			continue ;
		}
		buf = expand_buffer(buf, &capacity, len);
		if (!buf)
			return (NULL);
		buf[len++] = c;
	}
}

/*
 * main - Main shell loop
 */
int	main(int argc, char **argv, char **envp)
{
	char				*line;
	struct s_cmd		*cmd;
	struct s_execcmd	*ecmd;
	int					status;

	(void)argc;
	(void)argv;
	(void)envp;
	setup_signals();
	while (1)
	{
		g_sig.sigint = 0;
		g_sig.sigquit = 0;
		g_sig.pid = 0;
		line = readline_helper();
		if (!line)
		{
			display_prompt();
			continue ;
		}
		cmd = tokenize(line);
		if (!cmd)
		{
			free(line);
			continue ;
		}
		if (cmd->type == EXEC)
		{
			ecmd = (struct s_execcmd *)cmd;
			if (ecmd->av[0] && is_builtin(ecmd->av[0]))
			{
				status = handle_builtin(ecmd->av);
				set_exit_status(status);
				free(line);
				continue ;
			}
		}
		g_sig.pid = forkk();
		if (g_sig.pid == 0)
		{
			runcmd(cmd);
			exit(0);
		}
		wait(&status);
		if (WIFSIGNALED(status))
		{
			set_exit_status(128 + WTERMSIG(status));
		}
		else
		{
			set_exit_status(WEXITSTATUS(status));
		}
		free(line);
	}
	return (g_last_exit_status);
}
