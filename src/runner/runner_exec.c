#include "minishell.h"

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
char	*find_command(const char *cmd)
{
	char	*path;
	char	*curr;
	char	*next;
	char	full_path[1024];
	size_t	len;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	curr = path;
	while (curr && *curr)
	{
		next = ft_strchr(curr, ':');
		if (next)
			len = (size_t)(next - curr);
		else
			len = ft_strlen(curr);
		if (len + ft_strlen(cmd) + 2 > sizeof(full_path))
		{
			fprintf(stderr, "Path too long: %.*s/%s\n", (int)len, curr, cmd);
			return (NULL);
		}
		ft_strlcpy(full_path, curr, len + 1);
		full_path[len] = '/';
		ft_strlcpy(full_path + len + 1, cmd, sizeof(full_path) - len - 1);
		if (access(full_path, X_OK) == 0)
			return (ft_strdup(full_path));
		if (!next)
			break ;
		curr = next + 1;
	}
	return (NULL);
}

/*
 * reset_signals - Reset signals to default behavior for child processes
 */
static void	reset_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/*
 * handle_exec_cmd - Handle EXEC command type
 *
 * @cmd: Command structure to execute
 */
void	handle_exec_cmd(struct s_cmd *cmd)
{
	struct s_execcmd	*ex;
	char				*cmd_path;
	extern char			**environ;
	int					status;

	ex = (struct s_execcmd *)cmd;
	if (ex->av[0] == 0)
		exit(0);
	if (is_builtin(ex->av[0]))
	{
		status = handle_builtin(ex->av);
		set_exit_status(status);
		if (cmd->type != LIST)
			exit(status);
		return ;
	}
	cmd_path = find_command(ex->av[0]);
	if (cmd_path)
	{
		reset_signals();
		execve(cmd_path, ex->av, environ);
		perror("execve failed");
		free(cmd_path);
		exit(1);
	}
	else
	{
		fprintf(stderr, "command not found: %s\n", ex->av[0]);
		exit(127);
	}
}

/*
 * handle_redir_cmd - Handle REDIR command type
 *
 * @cmd: Command structure to execute
 */
void	handle_redir_cmd(struct s_cmd *cmd)
{
	struct s_redircmd	*rdir;
	int					fd;

	rdir = (struct s_redircmd *)cmd;
	close(rdir->fd);
	fd = open(rdir->file, rdir->mode, 0644);
	if (fd < 0)
	{
		fprintf(stderr, "open failed: %s: %s\n", rdir->file, strerror(errno));
		exit(1);
	}
	if (fd != rdir->fd)
	{
		dup2(fd, rdir->fd);
		close(fd);
	}
	if (ft_strnstr(rdir->file, "/tmp/minishell_heredoc_",
			ft_strlen(rdir->file)) != NULL)
	{
		unlink(rdir->file);
	}
	runcmd(rdir->cmd);
}
