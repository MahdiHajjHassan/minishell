#include "minishell.h"

/*
 * handle_escape_char - Handle single escaped character
 *
 * @input: Input character after backslash
 * @output: Output buffer
 * @j: Position in output buffer
 *
 * Returns: 1 if character was handled, 0 otherwise
 */
static int	handle_escape_char(char input, char *output, size_t *j)
{
	if (input == 'n')
		output[(*j)++] = '\n';
	else if (input == 't')
		output[(*j)++] = '\t';
	else if (input == 'r')
		output[(*j)++] = '\r';
	else if (input == 'v')
		output[(*j)++] = '\v';
	else if (input == 'b')
		output[(*j)++] = '\b';
	else if (input == 'f')
		output[(*j)++] = '\f';
	else if (input == 'a')
		output[(*j)++] = '\a';
	else if (input == '\\')
		output[(*j)++] = '\\';
	else
	{
		output[(*j)++] = input;
	}
	return (1);
}

/*
 * process_escaped - Handle escaped characters in input
 *
 * Processes escape sequences like \n, \t, etc. and converts them
 * to their actual characters. Also handles backslash escaping.
 *
 * @input: Input string containing escape sequences
 * @len: Length of input string
 *
 * Returns: Newly allocated string with processed escapes
 */
char	*process_escaped(const char *input, size_t len)
{
	char	*output;
	size_t	i;
	size_t	j;

	output = malloc(len + 1);
	i = 0;
	j = 0;
	if (!output)
		return (NULL);
	while (i < len)
	{
		if (input[i] == '\\' && i + 1 < len)
		{
			handle_escape_char(input[i + 1], output, &j);
			i += 2;
		}
		else
			output[j++] = input[i++];
	}
	output[j] = '\0';
	return (output);
}
