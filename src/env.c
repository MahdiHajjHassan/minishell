#include "minishell.h"

/* Global variable to store last command's exit status */
int				g_last_exit_status = 0;

/*
 * set_exit_status - Update the last command's exit status
 *
 * Extracts the actual exit status from the wait status value
 * and stores it in the global variable.
 *
 * @status: Status value from wait() system call
 */
void	set_exit_status(int status)
{
	g_last_exit_status = status;
}

/*
 * get_var_name_len - Get length of environment variable name
 *
 * Determines the length of a variable name in a string.
 * Handles special case of $? for exit status.
 * Valid variable names contain alphanumeric chars and underscore.
 *
 * @str: String starting with variable name
 *
 * Returns: Length of variable name
 */
static size_t	get_var_name_len(const char *str)
{
	size_t	len;

	len = 0;
	if (*str == '?') // Special case for $?
		return (1);
	while (str[len] && (isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

/*
 * get_env_value - Get value of environment variable
 *
 * Retrieves value of environment variable or exit status.
 * Handles special $? variable for last command's exit status.
 *
 * @name: Name of variable
 * @name_len: Length of variable name
 *
 * Returns:
 *   - Newly allocated string with variable value
 *   - Empty string if variable not found
 *   - NULL on memory allocation failure
 */
static char	*get_env_value(const char *name, size_t name_len)
{
	char	*temp;
	char	*value;
	char	*num;

	if (name_len == 1 && *name == '?')
	{
		// Handle $? special case
		num = ft_itoa(g_last_exit_status);
		if (!num)
			return (ft_strdup("0"));
		return (num);
	}
	// Create null-terminated copy of variable name
	temp = malloc(name_len + 1);
	if (!temp)
		return (NULL);
	ft_strlcpy(temp, name, name_len + 1);
	temp[name_len] = '\0';
	value = getenv(temp);
	free(temp);
	return (value ? ft_strdup(value) : ft_strdup(""));
}

/*
 * handle_brace_expansion - Handle ${var} syntax expansion
 *
 * @str: Input string
 * @i: Current position in string
 * @len: Length of string
 * @result: Result buffer
 * @j: Position in result buffer
 * @alloc_size: Allocated size of result buffer
 *
 * Returns: Updated position in input string
 */
static size_t	handle_brace_expansion(const char *str, size_t i, size_t len,
		char **result, size_t *j, size_t *alloc_size)
{
	size_t	var_name_len;
	char	*env_value;
	size_t	start;
	size_t	value_len;
	char	*new_result;

	i += 2; // Skip ${
	start = i;
	// Find the closing }
	while (i < len && str[i] != '}')
		i++;
	if (i >= len || str[i] != '}')
	{
		// No closing brace found, treat as literal
		(*result)[(*j)++] = '$';
		(*result)[(*j)++] = '{';
		return (start);
	}
	var_name_len = i - start;
	if (var_name_len > 0)
	{
		env_value = get_env_value(str + start, var_name_len);
		if (env_value)
		{
			value_len = ft_strlen(env_value);
			// Ensure we have enough space
			if (*j + value_len >= *alloc_size)
			{
				*alloc_size = (*j + value_len) * 2;
				new_result = realloc(*result, *alloc_size);
				if (!new_result)
				{
					free(*result);
					free(env_value);
					return (0);
				}
				*result = new_result;
			}
			ft_strlcpy(*result + *j, env_value, *alloc_size - *j);
			*j += value_len;
			free(env_value);
		}
	}
	i++; // Skip the closing }
	return (i);
}

/*
 * handle_simple_expansion - Handle $var syntax expansion
 *
 * @str: Input string
 * @i: Current position in string
 * @result: Result buffer
 * @j: Position in result buffer
 * @alloc_size: Allocated size of result buffer
 *
 * Returns: Updated position in input string
 */
static size_t	handle_simple_expansion(const char *str, size_t i,
		char **result, size_t *j, size_t *alloc_size)
{
	size_t	var_name_len;
	char	*env_value;
	size_t	value_len;
	char	*new_result;

	i++; // Skip the $
	var_name_len = get_var_name_len(str + i);
	env_value = get_env_value(str + i, var_name_len);
	if (env_value)
	{
		value_len = ft_strlen(env_value);
		// Ensure we have enough space
		if (*j + value_len >= *alloc_size)
		{
			*alloc_size = (*j + value_len) * 2;
			new_result = realloc(*result, *alloc_size);
			if (!new_result)
			{
				free(*result);
				free(env_value);
				return (0);
			}
			*result = new_result;
		}
		ft_strlcpy(*result + *j, env_value, *alloc_size - *j);
		*j += value_len;
		free(env_value);
	}
	i += var_name_len;
	return (i);
}

/*
 * handle_variable_expansion - Handle variable expansion for $var or ${var}
 *
 * @str: Input string
 * @i: Current position in string
 * @len: Length of string
 * @result: Result buffer
 * @j: Position in result buffer
 * @alloc_size: Allocated size of result buffer
 *
 * Returns: Updated position in input string
 */
static size_t	handle_variable_expansion(const char *str, size_t i, size_t len,
		char **result, size_t *j, size_t *alloc_size)
{
	char	*new_result;

	if (str[i + 1] == '{')
	{
		return (handle_brace_expansion(str, i, len, result, j, alloc_size));
	}
	else if (isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?')
	{
		return (handle_simple_expansion(str, i, result, j, alloc_size));
	}
	else
	{
		// $ not followed by valid variable character, treat as literal
		if (*j + 1 >= *alloc_size)
		{
			*alloc_size *= 2;
			new_result = realloc(*result, *alloc_size);
			if (!new_result)
			{
				free(*result);
				return (0);
			}
			*result = new_result;
		}
		(*result)[(*j)++] = str[i++];
	}
	return (i);
}

/*
 * handle_literal_character - Handle literal character in expansion
 *
 * @str: Input string
 * @i: Current position in string
 * @result: Result buffer
 * @j: Position in result buffer
 * @alloc_size: Allocated size of result buffer
 *
 * Returns: Updated position in input string
 */
static size_t	handle_literal_character(const char *str, size_t i,
		char **result, size_t *j, size_t *alloc_size)
{
	char	*new_result;

	if (*j + 1 >= *alloc_size)
	{
		*alloc_size *= 2;
		new_result = realloc(*result, *alloc_size);
		if (!new_result)
		{
			free(*result);
			return (0);
		}
		*result = new_result;
	}
	(*result)[(*j)++] = str[i++];
	return i;
}

/*
 * expand_variables - Expand environment variables in string
 *
 * Replaces all environment variables ($NAME) with their values.
 * Also handles the special $? variable for exit status.
 *
 * Example:
 *   Input:  "Hello $USER, status=$?"
 *   Output: "Hello john, status=0"
 *
 * @str: Input string containing variables to expand
 * @len: Length of input string
 *
 * Returns:
 *   - New string with variables expanded
 *   - NULL on memory allocation failure
 */
char	*expand_variables(const char *str, size_t len)
{
	char *result;
	size_t i = 0;
	size_t j = 0;
	size_t alloc_size = len * 2; // Initial estimate for space needed

	result = malloc(alloc_size);
	if (!result)
		return NULL;

	while (i < len)
	{
		if (str[i] == '$' && i + 1 < len)
		{
			i = handle_variable_expansion(str, i, len, &result, &j,
					&alloc_size);
			if (i == 0)
				return NULL;
		}
		else
		{
			i = handle_literal_character(str, i, &result, &j, &alloc_size);
			if (i == 0)
				return NULL;
		}
	}
	result[j] = '\0';

	// Shrink to actual size needed
	char *final = realloc(result, j + 1);
	return final ? final : result;
}