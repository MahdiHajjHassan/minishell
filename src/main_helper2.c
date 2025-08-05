/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_tokenize(char *line, struct s_cmd **cmd, char **env_copy)
{
	*cmd = tokenize(line, env_copy);
	if (! *cmd)
		return (1);
	return (0);
}

void	execute_cmd(struct s_cmd *cmd, char **env_copy)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		set_exit_status(1);
		return ;
	}
	if (pid == 0)
	{
		runcmd(cmd, env_copy);
		clean_exit(get_exit_status());
	}
	waitpid(pid, &status, 0);
	handle_child_status(status);
}
