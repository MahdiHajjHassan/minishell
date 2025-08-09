/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_argument_with_expansion(char *q, char *eq, char **env_copy,
		int quote_type)
{
	size_t	len;
	char	*processed;

	len = eq - q;
	processed = process_quoted_string(q, len, quote_type);
	if (! processed)
		return (NULL);
	if (quote_type != '\'')
	{
		if (check_for_variables(processed))
		{
			return (expand_if_needed(processed, env_copy));
		}
	}
	return (processed);
}
