#include "minishell.h"

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
	*equals = '\0';
	value = equals + 1;
	if (setenv(name, value, 1) != 0)
	{
		fprintf(stderr, "setenv failed\n");
		wtf();
	}
	return (1);
}

static void	process_escape_sequence(const char **in, char **out)
{
	char	next_char;

	next_char = *(*in + 1);
	if (next_char == 'n')
		*(*out)++ = '\n';
	else if (next_char == 't')
		*(*out)++ = '\t';
	else if (next_char == 'r')
		*(*out)++ = '\r';
	else if (next_char == 'v')
		*(*out)++ = '\v';
	else if (next_char == 'b')
		*(*out)++ = '\b';
	else if (next_char == 'f')
		*(*out)++ = '\f';
	else if (next_char == 'a')
		*(*out)++ = '\a';
	else if (next_char == '\\')
		*(*out)++ = '\\';
	else
		*(*out)++ = next_char;
	*in += 2;
}

static char	*process_quotes_robust(char *input, int len)
{
	char		*result;
	char		*out;
	const char	*in;
	char		quote_char;
	int			in_quotes;

	result = malloc(len * 8 + 1);
	if (!result)
		return (NULL);
	out = result;
	in = input;
	in_quotes = 0;
	quote_char = 0;
	while (*in)
	{
		if (!in_quotes && (*in == '\'' || *in == '\"'))
		{
			in_quotes = 1;
			quote_char = *in++;
		}
		else if (in_quotes && *in == quote_char)
		{
			in_quotes = 0;
			quote_char = 0;
			in++;
		}
		else if (*in == '\\' && *(in + 1))
			process_escape_sequence(&in, &out);
		else
			*out++ = *in++;
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

struct s_cmd	*parseexec(char **input_ptr, char *input_end)
{
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;
	char				*q;
	char				*eq;
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
		tok = gettoken(input_ptr, input_end, &q, &eq);
		if (tok == 0)
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
		cmd->av[argc++] = expanded;
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
