/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper8.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cd_error(const char *path, const char *error)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd((char *)error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_cd_home_not_set(void)
{
	ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
}

void	print_cd_getcwd_failed(void)
{
	ft_putstr_fd("minishell: cd: getcwd failed\n", STDERR_FILENO);
}

void	print_cd_too_many_args(void)
{
	ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
}

void	print_export_invalid_identifier(const char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}
