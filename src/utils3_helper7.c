/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_error(const char *error)
{
	ft_putstr_fd("minishell: export: ", STDERR_FILENO);
	ft_putstr_fd((char *)error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_path_too_long(const char *curr, const char *cmd)
{
	ft_putstr_fd("Path too long: ", STDERR_FILENO);
	ft_putstr_fd((char *)curr, STDERR_FILENO);
	ft_putstr_fd("/", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_open_failed(const char *file, const char *error)
{
	ft_putstr_fd("open failed: ", STDERR_FILENO);
	ft_putstr_fd((char *)file, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd((char *)error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_dup2_failed(const char *error)
{
	ft_putstr_fd("dup2 failed: ", STDERR_FILENO);
	ft_putstr_fd((char *)error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_exit_numeric_arg(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}
