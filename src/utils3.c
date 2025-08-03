/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	(void)str;
	(void)size;
	(void)format;
	return (0);
}

int	ft_setenv(const char *name, const char *value, int overwrite,
		char ***env_copy)
{
	int		i;
	int		name_len;

	if (! name || ! *name || ft_strchr(name, '=') || ! env_copy || ! *env_copy)
		return (-1);
	name_len = ft_strlen(name);
	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], name, name_len) == 0
			&& (*env_copy)[i][name_len] == '=')
		{
			if (! overwrite)
				return (0);
			return (update_existing_var(env_copy, i, name, value));
		}
		i++;
	}
	return (add_new_var(env_copy, name, value));
}

int	ft_unsetenv(const char *name, char ***env_copy)
{
	int		name_len;
	int		var_index;

	if (! name || ! *name || ft_strchr(name, '=') || ! env_copy || ! *env_copy)
		return (-1);
	name_len = ft_strlen(name);
	var_index = find_var_index(env_copy, name, name_len);
	if (var_index == -1)
		return (0);
	return (remove_var_from_env(env_copy, var_index));
}
