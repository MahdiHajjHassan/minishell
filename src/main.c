#include "minishell.h"
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

t_sig	g_sig = {0, 0, 0, 0};

static void	display_prompt(void)
{
	char	cwd[1024];

	if (isatty(STDIN_FILENO))
	{
		if (get_cwd(cwd, sizeof(cwd)))
		{
			printf("%s> ", cwd);
			fflush(stdout);
		}
	}
}

static void	sigint_handler(int signo)
{
	(void)signo;
	if (g_sig.pid == 0)
	{
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

static int	handle_eof(char *buf, size_t len)
{
	if (len == 0)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		free(buf);
		exit(g_last_exit_status);
	}
	clearerr(stdin);
	return (1);
}

static void	handle_backspace(size_t *len)
{
	if (*len > 0)
	{
		(*len)--;
		if (isatty(STDIN_FILENO))
		{
			printf("\b \b");
			fflush(stdout);
		}
	}
}

static char	*resize_buffer(char *buf, size_t *capacity)
{
	char	*new_buf;

	*capacity *= 2;
	new_buf = realloc(buf, *capacity);
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	return (new_buf);
}

static char	*init_buffer(size_t *capacity, size_t *len)
{
	char	*buf;

	buf = NULL;
	*capacity = 128;
	*len = 0;
	buf = malloc(*capacity);
	if (!buf)
		return (NULL);
	display_prompt();
	return (buf);
}

static int	process_char(int c, char **buf, size_t *capacity, size_t *len)
{
	if (c == EOF)
	{
		handle_eof(*buf, *len);
		(*buf)[*len] = '\0';
		return (0);
	}
	if (c == '\n')
	{
		(*buf)[*len] = '\0';
		return (1);
	}
	if (c == 127 || c == 8)
	{
		handle_backspace(len);
		return (0);
	}
	if (*len + 1 >= *capacity)
	{
		*buf = resize_buffer(*buf, capacity);
		if (!*buf)
			return (-1);
	}
	(*buf)[(*len)++] = c;
	return (0);
}

char	*readline_helper(void)
{
	char	*buf;
	size_t	capacity;
	size_t	len;
	int		c;
	int		result;

	buf = init_buffer(&capacity, &len);
	if (!buf)
		return (NULL);
	while (1)
	{
		c = getchar();
		result = process_char(c, &buf, &capacity, &len);
		if (result == 1)
			return (buf);
		if (result == -1)
			return (NULL);
	}
}

static void	init_signals(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
}

static int	handle_line_input(char **line)
{
	*line = readline_helper();
	if (!*line)
	{
		display_prompt();
		return (1);
	}
	return (0);
}

static int	handle_tokenize(char *line, struct s_cmd **cmd)
{
	*cmd = tokenize(line);
	if (!*cmd)
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	expand_builtin_args(struct s_execcmd *ecmd)
{
	for (int i = 0; ecmd->av[i]; i++)
	{
		char	*original;

		original = ecmd->av[i];
		ecmd->av[i] = expand_variables(original, strlen(original));
		free(original);
	}
}

static int	handle_builtin_cmd(struct s_cmd *cmd, char *line)
{
	struct s_execcmd	*ecmd;
	int					status;

	if (cmd->type == EXEC)
	{
		ecmd = (struct s_execcmd *)cmd;
		if (ecmd->av[0] && is_builtin(ecmd->av[0]))
		{
			expand_builtin_args(ecmd);
			status = handle_builtin(ecmd->av);
			set_exit_status(status);
			free(line);
			return (1);
		}
	}
	return (0);
}

static void	handle_child_status(int status)
{
	if (WIFSIGNALED(status))
	{
		set_exit_status(128 + WTERMSIG(status));
	}
	else
	{
		set_exit_status(WEXITSTATUS(status));
	}
}

static void	execute_cmd(struct s_cmd *cmd)
{
	int	status;

	g_sig.pid = forkk();
	if (g_sig.pid == 0)
	{
		runcmd(cmd);
		exit(0);
	}
	wait(&status);
	handle_child_status(status);
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	struct s_cmd	*cmd;

	(void)argc;
	(void)argv;
	(void)envp;
	setup_signals();
	while (1)
	{
		init_signals();
		if (handle_line_input(&line))
			continue;
		if (handle_tokenize(line, &cmd))
			continue;
		if (handle_builtin_cmd(cmd, line))
			continue;
		execute_cmd(cmd);
		free(line);
	}
	return (g_last_exit_status);
}
