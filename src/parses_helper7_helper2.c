/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper7_helper2.c                           :+:      :+:    :+:   */
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

int	is_assignment_word(const char *s)
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

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

void	add_split_arguments(struct s_execcmd *cmd,
	int *argc, char *processed)
{
	size_t	i;
	size_t	start;
	size_t	len;
	char	*field;

	i = 0;
	while (processed[i])
	{
		while (processed[i] && is_whitespace(processed[i]))
			i++;
		start = i;
		while (processed[i] && !is_whitespace(processed[i]))
			i++;
		len = i - start;
		if (len > 0)
		{
			field = ft_substr(processed, start, len);
			if (!field)
			{
				print_malloc_failed();
				break ;
			}
			add_argument(cmd, field, argc);
		}
	}
}
