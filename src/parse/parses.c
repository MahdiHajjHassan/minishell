#include "minishell.h"

/*
 * handle_variable_assignment - Handle variable assignment in command
 *
 * @expanded: Expanded argument string
 * @argc: Current argument count
 *
 * Returns: 1 if handled as assignment, 0 otherwise
 */
static int	handle_variable_assignment(char *expanded, int argc)
{
	char	*equals;
	char	*name;
	char	*value;
	int		is_valid_var;
	char	*p;

	if (ft_strchr(expanded, '=') == NULL || argc != 0)
		return (0);
	equals = ft_strchr(expanded, '=');
	name = expanded;
	is_valid_var = 1;
	p = name;
	while (p < equals)
	{
		if (!isalnum(*p) && *p != '_')
		{
			is_valid_var = 0;
			break ;
		}
		p++;
	}
	if (!is_valid_var || name >= equals)
		return (0);
	*equals = '\0'
	value = equals + 1;
	if (setenv(name, value, 1) != 0)
	{
		fprintf(stderr, "setenv failed\n");
		wtf();
	}
	return (1);
}

/*
 * process_quotes_robust - Robust quote processing with concatenation support
 *
 * Handles complex quote scenarios including:
 * - Quote concatenation: 'a'"b"'c' -> abc
 * - Mixed quotes with variables: '"'"$USER"'"' -> "mahdi"
 * - Proper variable expansion in different quote contexts
 * - Edge cases like empty quotes, unmatched quotes
 *
 * @input: Input string with quotes
 * @len: Length of input string
 *
 * Returns: Processed string with quotes handled and variables expanded
 */
static char	*process_quotes_robust(char *input, int len)
{
	char	*out;
	char	*in;
	char	*end;
	char	quote_char;
	int		in_quotes;
	char	next_char;
	char	*var_start;
	char	*var_end;
	char	var_buffer[256];
	int		var_len;
	char	*expanded;
	char	next_char;
	char	*var_start;
	char	*var_end;
	char	var_buffer[256];
	int		var_len;
	char	*expanded;
	char	*result;

	result = malloc(len * 8 + 1);
	if (!result)
		return (NULL);
	out = result;
	in = input;
	end = input + len;
	quote_char = 0;
	in_quotes = 0;
	while (in < end)
	{
		if (!in_quotes)
		{
			if (*in == '"')
			{
				quote_char = '"';
				in_quotes = 1;
				in++;
			}
			else if (*in == '\'')
			{
				quote_char = '\'';
				in_quotes = 1;
				in++;
			}
			else
			{
				if (*in == '\\' && (in + 1) < end)
				{		
					next_char = *(in + 1);
					if (next_char == 'n')
						*out++ = '\n';
					else if (next_char == 't')
						*out++ = '\t';
					else if (next_char == 'r')
						*out++ = '\r';
					else if (next_char == 'v')
						*out++ = '\v';
					else if (next_char == 'b')
						*out++ = '\b';
					else if (next_char == 'f')
						*out++ = '\f';
					else if (next_char == 'a')
						*out++ = '\a';
					else if (next_char == '\\')
						*out++ = '\\';
					else
						*out++ = next_char;
					in += 2;
				}
				else if (*in == '$')
				{
					var_start = in;
					var_end = in + 1;
					if (var_end < end && *var_end == '{')
					{
						var_end++;
						while (var_end < end && *var_end != '}')
						{
							var_end++;
						}
						if (var_end < end && *var_end == '}')
						{
							var_end++;
						}
					}
					else if (var_end < end && (isalnum(*var_end)
							|| *var_end == '_'))
					{
						while (var_end < end && (isalnum(*var_end)
								|| *var_end == '_'))
						{
							var_end++;
						}
					}
					if (var_end > var_start + 1)
					{
						var_len = var_end - var_start;
						if (var_len < 255)
						{
							ft_strlcpy(var_buffer, var_start, var_len + 1);
							expanded = expand_variables(var_buffer, var_len);
							if (expanded)
							{
								ft_strlcpy(out, expanded, 256 - (out - result));
								out += ft_strlen(expanded);
								free(expanded);
							}
							in = var_end;
						}
						else
						{
							*out++ = *in++;
						}
					}
					else
						*out++ = *in++;
				}
				else
					*out++ = *in++;
			}
		}
		else
		{
			if (*in == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
				in++;
			}
			else
			{
				if (quote_char == '"' && *in == '\\' && (in + 1) < end)
				{
					next_char = *(in + 1);
					if (next_char == '"' || next_char == '\\'
						|| next_char == '$')
					{	
						*out++ = next_char;
						in += 2
					}
					else
					{
				
						*out++ = *in++;
					}
				}
				else if (quote_char == '"' && *in == '$')
				{
					var_start = in;
					var_end = in + 1;
					if (var_end < end && *var_end == '{')
					{
						var_end++;
						while (var_end < end && *var_end != '}')
						{
							var_end++;
						}
						if (var_end < end && *var_end == '}')
						{
							var_end++;
						}
					}
					else if (var_end < end && (isalnum(*var_end)
							|| *var_end == '_'))
					{
						while (var_end < end && (isalnum(*var_end)
								|| *var_end == '_'))
						{
							var_end++;
						}
					}
					if (var_end > var_start + 1)
					{
						var_len = var_end - var_start;
						if (var_len < 255)
						{
							ft_strlcpy(var_buffer, var_start, var_len + 1);
							expanded = expand_variables(var_buffer, var_len);
							if (expanded)
							{
								ft_strlcpy(out, expanded, 256 - (out - result));
								out += ft_strlen(expanded);
								free(expanded);
							}
							in = var_end;
						}
						else
						{
							*out++ = *in++;
						}
					}
					else
					{
						*out++ = *in++;
					}
				}
				else
					*out++ = *in++;
			}
		}
	}
	if (in_quotes)
	{
		free(result);
		fprintf(stderr, "minishell: syntax error: unclosed quote\n");
		return (NULL);
	}
	*out = '\0';
	return (result);
}

/*
 * parseexec - Parse a simple command with arguments
 *
 * Handles simple commands like: ls -l /home
 * Also processes argument escapes and environment variables
 *
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 *
 * Returns: Command tree for execution
 */
struct s_cmd	*parseexec(char **input_ptr, char *input_end)
{
	struct s_execcmd	*cmd;
	char				*q;
	char				*eq;
	struct s_cmd		*ret;
	int					argc;
	int					tok;
	size_t				len;
	char				*expanded;

	argc = 0;
	if (peek(input_ptr, input_end, "("))
		return (parse_block(input_ptr, input_end));
	ret = execcmd();
	cmd = (struct s_execcmd *)ret;
	ret = parse_redirs(ret, input_ptr, input_end);
	while (!peek(input_ptr, input_end, "|)&;"))
	{
		if ((tok = gettoken(input_ptr, input_end, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
		{
			fprintf(stderr, "missing file name\n");
			wtf();
		}
		len = eq - q;
		expanded = process_quotes_robust(q, len);
		if (!expanded)
			wtf();
		if (handle_variable_assignment(expanded, argc))
		{
			free(expanded);
			continue ;
		}
		cmd->av[argc] = expanded;
		argc++;
		if (argc >= MAXARGS)
		{
			fprintf(stderr, "too many args\n");
			wtf();
		}
		ret = parse_redirs(ret, input_ptr, input_end);
	}
	cmd->av[argc] = 0;
	return (ret);
}
