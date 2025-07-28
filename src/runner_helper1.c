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
			return (strdup(cmd));
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
			return (strdup(full_path));
		if (!next)
			break ;
		curr = next + 1;
	}
	return (NULL);
}

void	expand_exec_args(struct s_execcmd *ex)
{
	int		i;
	char	*original;

	i = 0;
	while (ex->av[i])
	{
		original = ex->av[i];
		ex->av[i] = expand_variables(original, strlen(original));
		free(original);
		i++;
	}
}

void	execute_external_cmd(struct s_execcmd *ex)
{
	char			*cmd_path;
	extern char		**environ;

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

void	run_pipe_cmd(struct s_cmd *cmd)
{
	int					p[2];
	struct s_pipecmd	*pipecmd;

	pipecmd = (struct s_pipecmd *)cmd;
	if (pipe(p) < 0)
	{
		perror("pipe failed");
		wtf();
	}
	setup_pipe_left(p, pipecmd);
	setup_pipe_right(p, pipecmd);
	close(p[0]);
	close(p[1]);
	wait(0);
	wait(0);
}
