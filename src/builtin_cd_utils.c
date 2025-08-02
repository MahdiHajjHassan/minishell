/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_to_home(char **env_copy)
{
	char	*home;
	int		i;

	i = 0;
	while (env_copy && env_copy[i])
	{
		if (ft_strncmp(env_copy[i], "HOME=", 5) == 0)
		{
			home = env_copy[i] + 5;
			if (chdir(home) != 0)
			{
				print_cd_error(home, strerror(errno));
				return (1);
			}
			return (0);
		}
		i++;
	}
	print_cd_home_not_set();
	return (1);
}

int	cd_to_path(char *path)
{
	if (chdir(path) != 0)
	{
		print_cd_error(path, strerror(errno));
		return (1);
	}
	return (0);
}
