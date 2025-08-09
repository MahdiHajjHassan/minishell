/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper6_helper4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_quote_loop(t_quote_params *params)
{
	while (*(params->i) < params->len)
	{
		if (*(params->i) < params->len - 1
			&& params->processed[*(params->i)] == '$'
			&& (params->processed[*(params->i) + 1] == '"'
				|| params->processed[*(params->i) + 1] == '\''))
		{
			process_dollar_quote_content(params);
		}
		else
		{
			params->result[*(params->j)] = params->processed[*(params->i)];
			(*(params->j))++;
			(*(params->i))++;
		}
	}
}

char	*process_dollar_quote_patterns(char *processed, size_t len)
{
	char			*result;
	size_t			i;
	size_t			j;
	t_quote_params	params;

	result = initialize_quote_result(processed, len);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	params = (t_quote_params){processed, len, result, &i, &j};
	process_quote_loop(&params);
	result[j] = '\0';
	free(processed);
	return (result);
}
