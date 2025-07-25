#include "minishell.h"

/* Characters considered as whitespace */
char			g_space[] = " \t\r\n\v";

/* Special shell syntax characters */
char			g_symbols[] = "<>|&;()";

/*
 * is_escaped - Check if a character is escaped with backslash
 *
 * Counts backslashes before the character to determine if it's escaped.
 * An odd number of backslashes means the character is escaped.
 *
 * @s: Current position in string
 * @start: Start of the string
 *
 * Returns: 1 if character is escaped, 0 otherwise
 */
static int	is_escaped(const char *s, const char *start)
{
	int	count;

	count = 0;
	while (s > start && *(s - 1) == '\\')
	{
		count++;
		s--;
	}
	return (count % 2);
}

/*
 * gettoken - Get next token from input string
 *
 * Tokenizes the input string based on shell syntax rules.
 * Handles special characters, whitespace, and escaped characters.
 *
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * @token_start: Where to store start of token
 * @token_end: Where to store end of token
 *
 * Returns: Token type:
 *   - 0 for end of string
 *   - Special character for operators (<,>,|,&,;)
 *   - '+' for >> operator
 *   - 'a' for regular arguments
 */
int	gettoken(char **input_ptr, char *input_end, char **token_start,
		char **token_end)
{
	char	*s;
	int		ret;
	char	quote;

	quote = 0;
	s = *input_ptr;
	while (s < input_end && strchr(g_space, *s))
		s++;
	if (token_start)
		*token_start = s;
	ret = *s;
	if (*s == 0)
	{
	}
	else if (*s == '|' || *s == '&' || *s == ';' || *s == '(' || *s == ')'
		|| *s == '<')
	{
		if (!is_escaped(s, *input_ptr))
			s++;
		else
		{
			if (*s == '>' && !is_escaped(s, *input_ptr))
			{
				s++;
				if (*s == '>' && !is_escaped(s, *input_ptr))
				{
					ret = '+';
					s++;
				}
			}
			else
			{
				ret = 'a';
			}
		}
	}
	else if (*s == '>')
	{
		if (!is_escaped(s, *input_ptr))
		{
			s++;
			if (*s == '>' && !is_escaped(s, *input_ptr))
			{
				ret = '+';
				s++;
			}
		}
		else
		{
			ret = 'a';
		}
	}
	else
	{
		ret = 'a';
		while (s < input_end)
		{
			if (!quote && !is_escaped(s, *input_ptr))
			{
				if (strchr(g_symbols, *s) || strchr(g_space, *s))
					break ;
				if (*s == '"' || *s == '\'')
					quote = *s;
			}
			else if (quote && !is_escaped(s, *input_ptr) && *s == quote)
				quote = 0;
			s++;
		}
	}
	if (token_end)
		*token_end = s;
	while (s < input_end && strchr(g_space, *s))
		s++;
	*input_ptr = s;
	return (ret);
}

/*
 * peek - Look ahead for specific tokens
 *
 * Checks if the next non-whitespace character matches any in toks.
 * Used to check for upcoming operators without consuming them.
 *
 * @input_ptr: Current position in input string
 * @input_end: End of input string
 * @toks: String of characters to look for
 *
 * Returns: 1 if next character matches any in toks, 0 otherwise
 */
int	peek(char **input_ptr, char *input_end, char *toks)
{
	char	*s;

	s = *input_ptr;
	while (s < input_end && strchr(g_space, *s))
		s++;
	return (*s && strchr(toks, *s));
}

/*
 * tokenize - Convert input line to command structure
 *
 * Main entry point for parsing shell commands.
 * Converts a string into a tree of command structures.
 *
 * @line: Input command line
 *
 * Returns: Root of command tree
 */
struct s_cmd	*tokenize(const char *line)
{
	char			*input;
	char			*input_ptr;
	char			*input_end;
	struct s_cmd	*cmd;

	input = ft_strdup(line);
	input_ptr = input;
	input_end = input + ft_strlen(input);
	cmd = parse_line(&input_ptr, input_end);
	peek(&input_ptr, input_end, "\0");
	if (input_ptr != input_end)
	{
		wtf();
	}
	free(input);
	return (cmd);
}
