/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin(char **argv, char ***env_copy)
{
	if (! argv[0])
		return (1);
	if (! ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	if (! ft_strcmp(argv[0], "cd"))
		return (builtin_cd(argv, env_copy));
	if (! ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd(argv, env_copy));
	if (! ft_strcmp(argv[0], "export"))
		return (builtin_export(argv, env_copy));
	if (! ft_strcmp(argv[0], "unset"))
		return (builtin_unset(argv, env_copy));
	if (! ft_strcmp(argv[0], "env"))
		return (builtin_env(argv, env_copy));
	if (! ft_strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
