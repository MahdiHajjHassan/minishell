/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_var_name_len(const char *str)
{
	size_t	len;

	len = 0;
	if (*str == '?')
		return (1);
	while (str[len] && (isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

char	*get_env_value(const char *name, size_t name_len)
{
	char	*temp;
	char	*value;
	char	num[32];

	if (name_len == 1 && *name == '?')
	{
		ft_snprintf(num, sizeof(num), "%d", g_last_exit_status);
		return (ft_strdup(num));
	}
	temp = malloc(name_len + 1);
	if (!temp)
		return (NULL);
	ft_strncpy(temp, name, name_len);
	temp[name_len] = '\0';
	value = getenv(temp);
	free(temp);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}

int	handle_env_variable(t_env_var_params params)
{
	size_t	var_name_len;
	char	*env_value;
	size_t	value_len;

	(*params.i)++;
	var_name_len = get_var_name_len(params.str + *params.i);
	env_value = get_env_value(params.str + *params.i, var_name_len);
	if (env_value)
	{
		value_len = strlen(env_value);
		*params.result = resize_for_env_value(*params.result,
				params.alloc_size, *params.j, value_len);
		if (!*params.result)
		{
			free(env_value);
			return (1);
		}
		ft_strcpy(*params.result + *params.j, env_value);
		*params.j += value_len;
		free(env_value);
	}
	*params.i += var_name_len;
	return (0);
}

int	handle_regular_char(t_regular_char_params params)
{
	*params.result = resize_for_char(*params.result,
			params.alloc_size, *params.j);
	if (!*params.result)
		return (1);
	(*params.result)[(*params.j)++] = params.str[(*params.i)++];
	return (0);
}

int	process_character(t_process_char_params params)
{
	if (is_variable_char(params.str, *params.i, params.len))
	{
		if (handle_env_variable((t_env_var_params){params.str, params.i,
				params.result, params.j, params.alloc_size}))
			return (1);
	}
	else
	{
		if (handle_regular_char((t_regular_char_params){params.str, params.i,
				params.result, params.j, params.alloc_size}))
			return (1);
	}
	return (0);
}
