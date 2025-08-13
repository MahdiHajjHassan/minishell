/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper4_helper_helper2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_name_start(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

static int is_name_char(char c)
{
    return (is_name_start(c) || (c >= '0' && c <= '9'));
}

static int is_assignment_word_local(const char *s)
{
    const char *eq;

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

int	process_single_argument(t_arg_process_params arg_params)
{
	char	*processed;
	int		quote_type;
	char	*q_copy;
	char	*eq_copy;
    size_t  i;
    size_t  start;
    size_t  len;
    char    *field;
    int     should_split;

	q_copy = *arg_params.q;
	eq_copy = *arg_params.eq;
	quote_type = remove_exec_quotes(&q_copy, &eq_copy);
    processed = process_argument_with_expansion(q_copy,
            eq_copy, arg_params.env_copy, quote_type);
	if (!processed)
	{
		free_cmd(*arg_params.ret);
		return (1);
	}
    should_split = (quote_type == 0
        && !is_assignment_word_local(processed)
        && (ft_strchr(processed, ' ') || ft_strchr(processed, '\t')));
    if (should_split)
    {
        i = 0;
        while (processed[i])
        {
            while (processed[i] && (processed[i] == ' ' || processed[i] == '\t'))
                i++;
            start = i;
            while (processed[i] && processed[i] != ' ' && processed[i] != '\t')
                i++;
            len = i - start;
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
        }
        free(processed);
    }
    else
    {
        add_argument(arg_params.params.cmd, processed, arg_params.params.argc);
    }
    return (0);
}

int	handle_quoted_argument_token(t_arg_process_params arg_params)
{
	char	*next_pos;

	next_pos = *arg_params.eq;
	while (next_pos < arg_params.params.input_end
		&& (*next_pos == ' ' || *next_pos == '\t'))
		next_pos++;
    if (next_pos == *arg_params.eq && next_pos < arg_params.params.input_end
        && *next_pos != '|'
        && *next_pos != '&' && *next_pos != ';' && *next_pos != '<'
        && *next_pos != '>')
		return (concatenate_adjacent_tokens(arg_params));
	return (process_single_argument(arg_params));
}

int	handle_unquoted_argument_token(t_arg_process_params arg_params)
{
	char	*next_pos;

	next_pos = *arg_params.eq;
	while (next_pos < arg_params.params.input_end
		&& (*next_pos == ' ' || *next_pos == '\t'))
		next_pos++;
    if (next_pos == *arg_params.eq && next_pos < arg_params.params.input_end
        && *next_pos != '|'
        && *next_pos != '&' && *next_pos != ';' && *next_pos != '<'
        && *next_pos != '>')
		return (concatenate_adjacent_tokens(arg_params));
	return (process_single_argument(arg_params));
}
