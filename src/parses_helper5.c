/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_argument(struct s_execcmd *cmd, char *processed, int *argc)
{
	if (*argc >= MAXARGS)
	{
		print_too_many_args();
		free(processed);
		return ;
	}
	if (ft_strlen(processed) == 0)
	{
		free(processed);
		return ;
	}
	cmd->av[*argc] = processed;
	cmd->eav[*argc] = cmd->av[*argc] + ft_strlen(processed);
	(*argc)++;
}

void	finalize_exec_cmd(struct s_execcmd *cmd, int argc)
{
	cmd->av[argc] = 0;
	cmd->eav[argc] = 0;
}

char	*allocate_and_copy_result(char *result, char *temp)
{
	size_t	total_len;
	char	*new_result;

	total_len = ft_strlen(result) + ft_strlen(temp);
	new_result = malloc(total_len + 1);
	if (! new_result)
	{
		free(result);
		free(temp);
		return (NULL);
	}
	ft_strcpy(new_result, result);
	ft_strlcat(new_result, temp, total_len + 1);
	free(result);
	free(temp);
	return (new_result);
}

char	*process_quoted_segment(char *current_pos, char *input_end,
		char **env_copy, char *result)
{
	char	*q;
	char	*eq;
	char	*temp;
	int		quote_type;

	if (current_pos >= input_end)
	{
		free(result);
		return (NULL);
	}
	quote_type = *current_pos;
	q = current_pos + 1;
	eq = q;
	while (eq < input_end && *eq != quote_type)
		eq++;
	if (eq >= input_end)
		eq = input_end;
	temp = process_argument_with_expansion(q, eq, env_copy, quote_type);
	if (! temp)
	{
		free(result);
		return (NULL);
	}
	return (allocate_and_copy_result(result, temp));
}

char	*skip_whitespace_chars(char *current_pos, char *input_end)
{
	while (current_pos < input_end
		&& (*current_pos == ' ' || *current_pos == '\t'))
		current_pos++;
	return (current_pos);
}
