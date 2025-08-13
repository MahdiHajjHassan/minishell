/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper_helper4.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_name_start(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

static int	is_name_char(char c)
{
	return (is_name_start(c) || (c >= '0' && c <= '9'));
}

int	is_assignment_word_local(const char *s)
{
	const char	*eq;

	if (!s)
		return (0);
	eq = ft_strchr(s, '=');
	if (!eq || eq == s)
		return (0);
	if (!is_name_start(*s))
		return (0);
	while (s < eq)
	{
		if (!is_name_char(*s))
			return (0);
		s++;
	}
	return (1);
}

static int	extract_and_add_field(char *processed, size_t *i,
	t_arg_process_params arg_params)
{
	size_t	start;
	size_t	len;
	char	*field;

	while (processed[*i] && (processed[*i] == ' ' || processed[*i] == '\t'))
		(*i)++;
	start = *i;
	while (processed[*i] && processed[*i] != ' ' && processed[*i] != '\t')
		(*i)++;
	len = *i - start;
	if (len > 0)
	{
		field = ft_substr(processed, start, len);
		if (!field)
		{
			print_malloc_failed();
			free(processed);
			return (1);
		}
		add_argument(arg_params.params.cmd, field, arg_params.params.argc);
	}
	return (0);
}

int	split_and_add_arguments(char *processed,
	t_arg_process_params arg_params)
{
	size_t	i;

	i = 0;
	while (processed[i])
	{
		if (extract_and_add_field(processed, &i, arg_params))
			return (1);
	}
	return (0);
}
