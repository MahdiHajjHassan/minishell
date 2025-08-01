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

/* Custom vsnprintf implementation for simple cases */
int	ft_vsnprintf(char *str, size_t size, const char *format, va_list args)
{
	int		i;
	int		num;
	char	*num_str;

	i = 0;
	while (*format && i < (int)size - 1)
	{
		if (*format == '%' && *(format + 1) == 'd')
		{
			num = va_arg(args, int);
			num_str = ft_itoa(num);
			if (!num_str)
				return (-1);
			while (*num_str && i < (int)size - 1)
			{
				str[i++] = *num_str++;
			}
			free(num_str);
			format += 2;
		}
		else if (*format == '%' && *(format + 1) == 's')
		{
			char *s = va_arg(args, char *);
			for (int j = 0; s && s[j] && i < (int)size - 1; j++)
				str[i++] = s[j];
			format += 2;
		}
		else if (*format == '%' && *(format + 1) == 'c')
		{
			char c = va_arg(args, int);
			if (i < (int)size - 1)
				str[i++] = c;
			format += 2;
		}
		else
		{
			str[i++] = *format++;
		}
	}
	str[i] = '\0';
	return (i);
}

/* Custom realloc implementation */
void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

/* Custom strncpy implementation */
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

/* Custom strcpy implementation */
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

/* Custom fflush implementation for stdout */
int	ft_fflush_stdout(void)
{
	/* Force stdout to be flushed by writing a null byte */
	return (write(STDOUT_FILENO, "", 0));
}

/* Custom fprintf implementation for stderr */
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

/* Custom setenv implementation */
int	ft_setenv(const char *name, const char *value, int overwrite, char ***env_copy)
{
	int			i;
	int			name_len;
	char		*new_var;
	char		**new_environ;
	char		**old_environ;

	if (!name || !*name || ft_strchr(name, '=') || !env_copy || !*env_copy)
		return (-1);
	name_len = ft_strlen(name);
	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], name, name_len) == 0 && (*env_copy)[i][name_len] == '=')
		{
			if (!overwrite)
				return (0);
			/* Variable exists, just update it */
			new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
			if (!new_var)
				return (-1);
			ft_strcpy(new_var, name);
			ft_strlcat(new_var, "=", ft_strlen(name) + 2);
			ft_strlcat(new_var, value, ft_strlen(name) + ft_strlen(value) + 2);
			free((*env_copy)[i]);
			(*env_copy)[i] = new_var;
			return (0);
		}
		i++;
	}
	
	/* Variable doesn't exist, add it */
	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (!new_var)
		return (-1);
	ft_strcpy(new_var, name);
	ft_strlcat(new_var, "=", ft_strlen(name) + 2);
	ft_strlcat(new_var, value, ft_strlen(name) + ft_strlen(value) + 2);
	
	/* Count existing variables */
	i = 0;
	while ((*env_copy)[i])
		i++;
	
	/* Allocate new array */
	new_environ = malloc((i + 2) * sizeof(char *));
	if (!new_environ)
	{
		free(new_var);
		return (-1);
	}
	
	/* Copy existing variables */
	i = 0;
	while ((*env_copy)[i])
	{
		new_environ[i] = (*env_copy)[i];
		i++;
	}
	new_environ[i] = new_var;
	new_environ[i + 1] = NULL;
	
	/* Update pointer and free old array */
	old_environ = *env_copy;
	*env_copy = new_environ;
	free(old_environ);
	
	return (0);
}

/* Custom unsetenv implementation */
int	ft_unsetenv(const char *name, char ***env_copy)
{
	int			i;
	int			name_len;
	char		**new_environ;
	int			j;
	int			total_vars;
	int			var_index;

	if (!name || !*name || ft_strchr(name, '=') || !env_copy || !*env_copy)
		return (-1);
	name_len = ft_strlen(name);
	i = 0;
	while ((*env_copy)[i])
	{
		if (ft_strncmp((*env_copy)[i], name, name_len) == 0 && (*env_copy)[i][name_len] == '=')
		{
			var_index = i;
			break ;
		}
		i++;
	}
	if (!(*env_copy)[i])
		return (0);
	
	/* Count total variables */
	total_vars = 0;
	while ((*env_copy)[total_vars])
		total_vars++;
	
	/* Free the variable being removed */
	free((*env_copy)[var_index]);
	
	new_environ = malloc((total_vars) * sizeof(char *));
	if (!new_environ)
		return (-1);
	
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
	
	/* Free the old environment array */
	free(*env_copy);
	*env_copy = new_environ;
	return (0);
} 