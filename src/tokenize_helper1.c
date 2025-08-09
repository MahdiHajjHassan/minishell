/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_whitespace(char *s, char *input_end, char *space)
{
	while (s < input_end && ft_strchr(space, *s))
		s++;
	return (s);
}

int	handle_special_chars(char **s_ptr, char *input_ptr)
{
	int	ret;

	ret = handle_basic_symbols(s_ptr, input_ptr);
	if (ret != -1)
		return (ret);
	ret = handle_greater_than(s_ptr, input_ptr);
	if (ret != -1)
		return (ret);
	ret = handle_less_than(s_ptr, input_ptr);
	if (ret != -1)
		return (ret);
	return ('a');
}

char	*handle_quote_logic(char *s, char *quote_ptr)
{
	if (!*quote_ptr)
	{
		if (*s == '"' || *s == '\'')
			*quote_ptr = *s;
	}
	else if (*quote_ptr && *s == *quote_ptr)
	{
		*quote_ptr = 0;
		s++;
		return (s);
	}
	return (NULL);
}
