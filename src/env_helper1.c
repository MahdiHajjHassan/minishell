/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
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

char	*get_env_value(const char *name, size_t name_len, char **env_copy)
{
	char	*temp;
	char	*value;
	int		i;

	if (name_len == 1 && *name == '?')
	{
		char *status_str = ft_itoa(get_exit_status());
		if (!status_str)
			return (ft_strdup("0"));
		return (status_str);
	}
	
	temp = malloc(name_len + 1);
	if (!temp)
		return (ft_strdup(""));
	
	ft_strncpy(temp, name, name_len);
	temp[name_len] = '\0';
	
	/* Search in local environment copy */
	if (!env_copy)
	{
		free(temp);
		return (ft_strdup(""));
	}
	
	i = 0;
	while (env_copy[i])
	{
		if (ft_strncmp(env_copy[i], temp, name_len) == 0 && env_copy[i][name_len] == '=')
		{
			value = ft_strdup(env_copy[i] + name_len + 1);
			free(temp);
			return (value);
		}
		i++;
	}
	
	free(temp);
	return (ft_strdup(""));
}

int	handle_env_variable(t_env_var_params params, char **env_copy)
{
	size_t	var_name_len;
	char	*env_value;
	size_t	value_len;

	(*params.i)++;
	var_name_len = get_var_name_len(params.str + *params.i);
	env_value = get_env_value(params.str + *params.i, var_name_len, env_copy);
	
	if (env_value)
	{
		value_len = ft_strlen(env_value);
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
	else
	{
		/* Handle case where env_value allocation failed */
		return (1);
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
				params.result, params.j, params.alloc_size, params.env_copy}, params.env_copy))
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
