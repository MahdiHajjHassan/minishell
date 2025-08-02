/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_absolute_path(const char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return ((char *)1);
}

char	*search_in_paths(char *path, const char *cmd)
{
	char	*curr;
	char	*next;
	char	full_path[1024];
	size_t	len;

	curr = path;
	while (curr && *curr)
	{
		len = get_path_segment_len(curr, &next);
		if (build_full_path(full_path, curr, len, cmd))
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (ft_strdup(full_path));
		if (!next)
			break ;
		curr = next + 1;
	}
	return (NULL);
}

void	expand_exec_args(struct s_execcmd *ex, char **env_copy)
{
	int		i;
	char	*original;

	i = 0;
	while (ex->av[i])
	{
		original = ex->av[i];
		ex->av[i] = expand_variables(original, ft_strlen(original), env_copy);
		free(original);
		i++;
	}
}

void	execute_external_cmd(struct s_execcmd *ex, char **env_copy)
{
	char	*cmd_path;
	pid_t	pid;
	int		status;

	cmd_path = find_command(ex->av[0], env_copy);
	if (! cmd_path)
	{
		ft_fprintf_stderr("minishell: %s: command not found\n", ex->av[0]);
		set_exit_status(127);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(cmd_path);
		set_exit_status(1);
		return ;
	}
	if (pid == 0)
	{
		reset_signals();
		execve(cmd_path, ex->av, env_copy);
		perror("execve failed");
		free(cmd_path);
		clean_exit(1);
	}
	else
	{
		free(cmd_path);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			set_exit_status(WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				set_exit_status(130);
			else if (WTERMSIG(status) == SIGQUIT)
				set_exit_status(131);
			else
				set_exit_status(128 + WTERMSIG(status));
		}
		else
			set_exit_status(1);
	}
}

void	run_pipe_cmd(struct s_cmd *cmd, char **env_copy)
{
	int					p[2];
	struct s_pipecmd	*pipecmd;
	pid_t				pid1;
	pid_t				pid2;
	int					status1;
	int					status2;

	pipecmd = (struct s_pipecmd *)cmd;
	if (pipe(p) < 0)
	{
		perror("pipe failed");
		set_exit_status(1);
		return ;
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork failed");
		close(p[0]);
		close(p[1]);
		set_exit_status(1);
		return ;
	}
	if (pid1 == 0)
	{
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
		runcmd(pipecmd->left, env_copy);
		exit(get_exit_status());
	}
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork failed");
		close(p[0]);
		close(p[1]);
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
		set_exit_status(1);
		return ;
	}
	if (pid2 == 0)
	{
		close(p[1]);
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		runcmd(pipecmd->right, env_copy);
		exit(get_exit_status());
	}
	close(p[0]);
	close(p[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		set_exit_status(WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
	{
		if (WTERMSIG(status2) == SIGINT)
			set_exit_status(130);
		else if (WTERMSIG(status2) == SIGQUIT)
			set_exit_status(131);
		else
			set_exit_status(128 + WTERMSIG(status2));
	}
	else
		set_exit_status(1);
}

void	run_back_cmd(struct s_cmd *cmd, char **env_copy)
{
	struct s_backcmd	*backcmd;
	pid_t			pid;
	int				status;

	backcmd = (struct s_backcmd *)cmd;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		set_exit_status(1);
		return ;
	}
	if (pid == 0)
	{
		runcmd(backcmd->cmd, env_copy);
		exit(get_exit_status());
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			set_exit_status(130);
		else if (WTERMSIG(status) == SIGQUIT)
			set_exit_status(131);
		else
			set_exit_status(128 + WTERMSIG(status));
	}
	else
		set_exit_status(1);
}
