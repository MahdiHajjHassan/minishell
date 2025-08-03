/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	gettoken(char **input_ptr, char *input_end,
		char **token_start, char **token_end)
{
	char	*s;
	int		ret;
	char	space[6];
	char	symbols[8];

	init_token_arrays(space, symbols);
	setup_token_processing(input_ptr, input_end, token_start, &s);
	ret = *s;
	if (*s == 0)
		;
	else
		ret = handle_token_cases(&s, *input_ptr, input_end);
	if (ret == 0)
		return (0);
	if (token_end)
		*token_end = s;
	s = skip_whitespace(s, input_end, space);
	*input_ptr = s;
	return (ret);
}

int	peek(char **input_ptr, char *input_end, char *toks)
{
	char	*s;
	char	space[6];

	init_space_array(space);
	s = *input_ptr;
	while (s < input_end && ft_strchr(space, *s))
		s++;
	return (*s && ft_strchr(toks, *s));
}

struct s_cmd	*tokenize(const char *line, char **env_copy)
{
	char			*input;
	char			*input_ptr;
	char			*input_end;
	struct s_cmd	*cmd;

	input = ft_strdup(line);
	if (! input)
		return (NULL);
	input_ptr = input;
	input_end = input + ft_strlen(input);
	cmd = parse_line(&input_ptr, input_end, env_copy);
	peek(&input_ptr, input_end, "\0");
	if (input_ptr != input_end)
	{
		print_syntax_error();
		free_cmd(cmd);
		free(input);
		return (NULL);
	}
	free(input);
	return (cmd);
}
