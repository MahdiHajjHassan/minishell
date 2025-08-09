/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_helper5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_new_var(char ***env_copy, const char *name,
	const char *value)
{
	char	*new_var;
	char	**new_environ;
	int		count;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (! new_var)
		return (-1);
	ft_strcpy(new_var, name);
	ft_strlcat(new_var, "=", ft_strlen(name) + 2);
	ft_strlcat(new_var, value, ft_strlen(name) + ft_strlen(value) + 2);
	count = allocate_new_environ(env_copy, &new_var, &new_environ);
	if (count == -1)
		return (-1);
	copy_env_vars_for_export(env_copy, new_environ, count);
	finalize_new_environ(new_environ, count, new_var, env_copy);
	return (0);
}
