/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exit_too_many_args(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
}

void	print_command_not_found(const char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

void	print_syntax_error(void)
{
	ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
}

void	print_missing_file_name(void)
{
	ft_putstr_fd("minishell: missing file name\n", STDERR_FILENO);
}

void	print_malloc_failed(void)
{
	ft_putstr_fd("minishell: malloc failed\n", STDERR_FILENO);
}
