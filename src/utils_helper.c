/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_helper.c                                    :+:      :+:    :+:   */
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
	{
		perror("fork failed");
		return (-1);
	}
	return (pid);
}

void	handle_internal_error(void)
{
	ft_putstr_fd("minishell: internal error\n", STDERR_FILENO);
	clean_exit(1);
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
