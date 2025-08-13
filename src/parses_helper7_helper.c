/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parses_helper7_helper.c                            :+:      :+:    :+:   */
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

static int is_assignment_word(const char *s)
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

static int	is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}

static void	add_split_arguments(struct s_execcmd *cmd, int *argc, char *processed)
{
    size_t i;
    size_t start;
    size_t len;
    char   *field;

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

char	*handle_quote_in_loop(char **current_pos, char *input_end,
		char **env_copy, char *result)
{
	result = handle_quoted_string(current_pos, input_end, env_copy, result);
	if (! result)
		return (NULL);
	return (result);
}

char	*advance_position_safely(char **current_pos, char *prev_pos,
		char *input_end)
{
	if (*current_pos == prev_pos)
	{
		(*current_pos)++;
		if (*current_pos >= input_end)
			return (NULL);
	}
	return (*current_pos);
}

char	*concatenate_quoted_strings(char **input_ptr, char *input_end,
		char **env_copy)
{
	char	*result;
	char	*current_pos;

	result = initialize_concatenation(input_ptr);
	if (! result)
		return (NULL);
	current_pos = *input_ptr;
	result = process_concatenation_loop(result, current_pos, input_end,
			env_copy);
	if (! result)
		return (NULL);
	*input_ptr = current_pos;
	return (result);
}

int	process_expanded_argument(t_expanded_arg_params params)
{
	char	*processed;
	int		quote_type;

	quote_type = remove_exec_quotes(&params.q, &params.eq);
	processed = process_argument_with_expansion(params.q,
			params.eq, params.env_copy, quote_type);
	if (! processed)
	{
		free_cmd(params.ret);
		return (1);
	}
    if (quote_type == 0
        && !is_assignment_word(processed)
        && (ft_strchr(processed, ' ') || ft_strchr(processed, '\t')))
    {
        add_split_arguments(params.params.cmd, params.params.argc, processed);
        free(processed);
    }
    else
    {
        add_argument(params.params.cmd, processed, params.params.argc);
    }
	return (0);
}
