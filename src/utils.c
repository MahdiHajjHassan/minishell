/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*get_cwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
	{
		perror("getcwd is dead");
		return (NULL);
	}
	return (buf);
}

int	forkk(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		wtf();
	return (pid);
}

void	wtf(void)
{
	printf("wtf\n");
	exit(1);
}

void	handle_exec_case(struct s_cmd *cmd)
{
	int					i;
	struct s_execcmd	*ecmd;

	ecmd = (struct s_execcmd *)cmd;
	i = 0;
	while (ecmd->av[i])
	{
		*ecmd->eav[i] = 0;
		i++;
	}
}

struct s_cmd	*nulterm(struct s_cmd *cmd)
{
	if (cmd == 0)
		return (0);
	if (cmd->type == EXEC)
		handle_exec_case(cmd);
	else if (cmd->type == REDIR)
		handle_redir_case(cmd);
	else if (cmd->type == PIPE)
		handle_pipe_case(cmd);
	else if (cmd->type == LIST)
		handle_list_case(cmd);
	else if (cmd->type == BACK)
		handle_back_case(cmd);
	return (cmd);
}
