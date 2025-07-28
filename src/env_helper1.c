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
		snprintf(num, sizeof(num), "%d", g_last_exit_status);
		return (strdup(num));
	}
	temp = malloc(name_len + 1);
	if (!temp)
		return (NULL);
	strncpy(temp, name, name_len);
	temp[name_len] = '\0';
	value = getenv(temp);
	free(temp);
	if (value)
		return (strdup(value));
	else
		return (strdup(""));
}

char	*init_result_buffer(size_t len, size_t *alloc_size)
{
	char	*result;

	*alloc_size = len * 2;
	result = malloc(*alloc_size);
	if (!result)
		return (NULL);
	return (result);
}

char	*resize_for_env_value(char *result, size_t *alloc_size,
	size_t j, size_t value_len)
{
	char	*new_result;

	if (j + value_len >= *alloc_size)
	{
		*alloc_size = (j + value_len) * 2;
		new_result = realloc(result, *alloc_size);
		if (!new_result)
		{
			free(result);
			return (NULL);
		}
		result = new_result;
	}
	return (result);
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
		strcpy(*params.result + *params.j, env_value);
		*params.j += value_len;
		free(env_value);
	}
	*params.i += var_name_len;
	return (0);
}

char	*resize_for_char(char *result, size_t *alloc_size, size_t j)
{
	char	*new_result;

	if (j + 1 >= *alloc_size)
	{
		*alloc_size *= 2;
		new_result = realloc(result, *alloc_size);
		if (!new_result)
		{
			free(result);
			return (NULL);
		}
		result = new_result;
	}
	return (result);
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

int	is_variable_char(const char *str, size_t i, size_t len)
{
	if (str[i] == '$' && i + 1 < len
		&& (isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
		return (1);
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