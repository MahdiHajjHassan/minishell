#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int	g_last_exit_status = 0;

void	set_exit_status(int status)
{
	g_last_exit_status = status;
}

static size_t	get_var_name_len(const char *str)
{
	size_t	len;

	len = 0;
	if (*str == '?')
		return (1);
	while (str[len] && (isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

static char	*get_env_value(const char *name, size_t name_len)
{
	char	*temp;
	char	*value;

	if (name_len == 1 && *name == '?')
	{
		char	num[32];

		snprintf(num, sizeof(num), "%d", g_last_exit_status);
		return (strdup(num));
	}
	temp = malloc(name_len + 1);
	if (!temp)
		return (NULL);
	strncpy(temp, name, name_len);
	temp[name_len] = '\0';
	value = getenv(temp);
	free(temp);
	return (value ? strdup(value) : strdup(""));
}

char	*expand_variables(const char *str, size_t len)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	alloc_size;
	size_t	var_name_len;
	char	*env_value;

	i = 0;
	j = 0;
	alloc_size = len * 2;
	result = malloc(alloc_size);
	if (!result)
		return (NULL);
	while (i < len)
	{
		if (str[i] == '$' && i + 1 < len && 
			(isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
		{
			i++;
			var_name_len = get_var_name_len(str + i);
			env_value = get_env_value(str + i, var_name_len);
			if (env_value)
			{
				size_t	value_len;
				char	*new_result;

				value_len = strlen(env_value);
				if (j + value_len >= alloc_size)
				{
					alloc_size = (j + value_len) * 2;
					new_result = realloc(result, alloc_size);
					if (!new_result)
					{
						free(result);
						free(env_value);
						return (NULL);
					}
					result = new_result;
				}
				strcpy(result + j, env_value);
				j += value_len;
				free(env_value);
			}
			i += var_name_len;
		}
		else
		{
			char	*new_result;

			if (j + 1 >= alloc_size)
			{
				alloc_size *= 2;
				new_result = realloc(result, alloc_size);
				if (!new_result)
				{
					free(result);
					return (NULL);
				}
				result = new_result;
			}
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	char	*final;

	final = realloc(result, j + 1);
	return (final ? final : result);
} 