/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper1_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		new_result = ft_realloc(result, *alloc_size);
		if (!new_result)
		{
			free(result);
			return (NULL);
		}
		result = new_result;
	}
	return (result);
}

char	*resize_for_char(char *result, size_t *alloc_size, size_t j)
{
	char	*new_result;

	if (j + 1 >= *alloc_size)
	{
		*alloc_size *= 2;
		new_result = ft_realloc(result, *alloc_size);
		if (!new_result)
		{
			free(result);
			return (NULL);
		}
		result = new_result;
	}
	return (result);
}

int	is_variable_char(const char *str, size_t i, size_t len)
{
	if (str[i] == '$' && i + 1 < len
		&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
		return (1);
	return (0);
}
