/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_exit_status_var(void)
{
	char	*status_str;

	status_str = ft_itoa(get_exit_status());
	if (! status_str)
		return (ft_strdup("0"));
	return (status_str);
}

char	*search_env_variable(const char *name, size_t name_len,
		char **env_copy, char *temp)
{
	char	*value;
	int		i;

	(void)name;
	i = 0;
	while (env_copy[i])
	{
		if (ft_strncmp(env_copy[i], temp, name_len) == 0
			&& env_copy[i][name_len] == '=')
		{
			value = ft_strdup(env_copy[i] + name_len + 1);
			free(temp);
			return (value);
		}
		i++;
	}
	free(temp);
	return (NULL);
}

int	handle_env_value_found(t_env_var_params params, char *env_value,
		size_t var_name_len, char **env_copy)
{
	size_t	value_len;

	(void)env_copy;
	(void)var_name_len;
	value_len = ft_strlen(env_value);
	*params.result = resize_for_env_value(*params.result,
			params.alloc_size, *params.j, value_len);
	if (! *params.result)
	{
		free(env_value);
		return (1);
	}
	ft_strcpy(*params.result + *params.j, env_value);
	*params.j += value_len;
	free(env_value);
	return (0);
}

int	handle_env_value_not_found(t_env_var_params params,
		size_t var_name_len)
{
	(void)params;
	(void)var_name_len;
	return (0);
}
