/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by mahajj-h          #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdarg.h>

/* Custom strcmp implementation */
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* Custom snprintf implementation for simple cases */
int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	int		result;

	va_start(args, format);
	result = ft_vsnprintf(str, size, format, args);
	va_end(args);
	return (result);
}

static int	handle_d_format(char *str, int *i, int size, va_list args)
{
	int		num;
	char	*num_str;
	int		j;

	num = va_arg(args, int);
	num_str = ft_itoa(num);
	if (! num_str)
		return (-1);
	j = 0;
	while (num_str[j] && *i < size - 1)
	{
		str[*i] = num_str[j];
		(*i)++;
		j++;
	}
	free(num_str);
	return (0);
}

static int	handle_s_format(char *str, int *i, int size, va_list args)
{
	char	*s;
	int		j;

	s = va_arg(args, char *);
	j = 0;
	while (s && s[j] && *i < size - 1)
	{
		str[*i] = s[j];
		(*i)++;
		j++;
	}
	return (0);
}

static int	handle_c_format(char *str, int *i, int size, va_list args)
{
	char	c;

	c = va_arg(args, int);
	if (*i < size - 1)
	{
		str[*i] = c;
		(*i)++;
	}
	return (0);
}

static int	handle_format_specifier(char *str, int *i, int size,
	const char **format, va_list args)
{
	if (**format == '%' && *(*format + 1) == 'd')
	{
		if (handle_d_format(str, i, size, args) == -1)
			return (-1);
		*format += 2;
	}
	else if (**format == '%' && *(*format + 1) == 's')
	{
		handle_s_format(str, i, size, args);
		*format += 2;
	}
	else if (**format == '%' && *(*format + 1) == 'c')
	{
		handle_c_format(str, i, size, args);
		*format += 2;
	}
	else
	{
		str[*i] = **format;
		(*i)++;
		(*format)++;
	}
	return (0);
}

static int	process_format_loop(char *str, int *i, int size,
	const char **format, va_list args)
{
	while (**format && *i < size - 1)
	{
		if (handle_format_specifier(str, i, size, format, args) == -1)
			return (-1);
	}
	return (0);
}

int	ft_vsnprintf(char *str, size_t size, const char *format, va_list args)
{
	int		i;

	i = 0;
	if (process_format_loop(str, &i, (int)size, &format, args) == -1)
		return (-1);
	str[i] = '\0';
	return (i);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (! ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(size);
	if (! new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest;
	while (*src)
	{
		*ptr = *src;
		ptr++;
		src++;
	}
	*ptr = '\0';
	return (dest);
}

int	ft_fflush_stdout(void)
{
	return (write(STDOUT_FILENO, "", 0));
}

int	ft_fprintf_stderr(const char *format, ...)
{
	va_list	args;
	char	buffer[1024];
	int		len;

	va_start(args, format);
	len = ft_vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	if (len > 0)
		write(STDERR_FILENO, buffer, len);
	return (len);
}

static int	update_existing_var(char ***env_copy, int i, const char *name,
		const char *value)
{
	char	*new_var;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (! new_var)
		return (-1);
	ft_strcpy(new_var, name);
	ft_strlcat(new_var, "=", ft_strlen(name) + 2);
	ft_strlcat(new_var, value, ft_strlen(name) + ft_strlen(value) + 2);
	free((*env_copy)[i]);
	(*env_copy)[i] = new_var;
	return (0);
}

static int	count_env_vars(char ***env_copy)
{
	int	i;

	i = 0;
	while ((*env_copy)[i])
		i++;
	return (i);
}

static int	allocate_new_environ(char ***env_copy, char **new_var,
		char ***new_environ)
{
	int	count;

	count = count_env_vars(env_copy);
	*new_environ = malloc((count + 2) * sizeof(char *));
	if (! *new_environ)
	{
		free(*new_var);
		return (-1);
	}
	return (count);
}

static void	copy_existing_env_vars(char ***env_copy, char **new_environ,
		int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_environ[i] = (*env_copy)[i];
		i++;
	}
}

static void	finalize_new_environ(char **new_environ, int count,
	char *new_var,
		char ***env_copy)
{
	new_environ[count] = new_var;
	new_environ[count + 1] = NULL;
	free(*env_copy);
	*env_copy = new_environ;
}

static int	add_new_var(char ***env_copy, const char *name,
	const char *value)
{
	char	*new_var;
	char	**new_environ;
	int		count;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (! new_var)
		return (-1);
	ft_strcpy(new_var, name);
	ft_strlcat(new_var, "=", ft_strlen(name) + 2);
	ft_strlcat(new_var, value, ft_strlen(name) + ft_strlen(value) + 2);
	count = allocate_new_environ(env_copy, &new_var, &new_environ);
	if (count == -1)
		return (-1);
	copy_existing_env_vars(env_copy, new_environ, count);
	finalize_new_environ(new_environ, count, new_var, env_copy);
	return (0);
}

static int	count_total_vars(char ***env_copy)
{
	int	total_vars;

	total_vars = 0;
	while ((*env_copy)[total_vars])
		total_vars++;
	return (total_vars);
}

static void	copy_vars_except_index(char ***env_copy, char **new_environ,
		int var_index)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((*env_copy)[i])
	{
		if (i != var_index)
		{
			new_environ[j] = (*env_copy)[i];
			j++;
		}
		i++;
	}
	new_environ[j] = NULL;
}

static int	remove_var_from_env(char ***env_copy, int var_index)
{
	char	**new_environ;
	int		total_vars;

	total_vars = count_total_vars(env_copy);
	free((*env_copy)[var_index]);
	new_environ = malloc((total_vars) * sizeof(char *));
	if (! new_environ)
		return (-1);
	copy_vars_except_index(env_copy, new_environ, var_index);
	free(*env_copy);
	*env_copy = new_environ;
	return (0);
}

int	ft_setenv(const char *name, const char *value, int overwrite,
		char ***env_copy)
{
	int		i;
	int		name_len;

	if (! name || ! *name || ft_strchr(name, '=') || ! env_copy || ! *env_copy)
		return (-1);
	name_len = ft_strlen(name);
	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], name, name_len) == 0
			&& (*env_copy)[i][name_len] == '=')
		{
			if (! overwrite)
				return (0);
			return (update_existing_var(env_copy, i, name, value));
		}
		i++;
	}
	return (add_new_var(env_copy, name, value));
}

static int	find_var_index(char ***env_copy, const char *name, int name_len)
{
	int		i;

	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], name, name_len) == 0
			&& (*env_copy)[i][name_len] == '=')
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

int	ft_unsetenv(const char *name, char ***env_copy)
{
	int		name_len;
	int		var_index;

	if (! name || ! *name || ft_strchr(name, '=') || ! env_copy || ! *env_copy)
		return (-1);
	name_len = ft_strlen(name);
	var_index = find_var_index(env_copy, name, name_len);
	if (var_index == -1)
		return (0);
	return (remove_var_from_env(env_copy, var_index));
}
