#include "minishell.h"

/*
 * ft_strlen - Calculate the length of a string
 */
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len])
		len++;
	return (len);
}

/*
 * ft_strchr - Locate character in string
 */
char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

/*
 * ft_strncmp - Compare strings up to n characters
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (s1 != s2);
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*
 * ft_strdup - Duplicate a string
 */
char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/*
 * ft_strlcpy - Copy string with size limit
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	i;

	if (!src)
		return (0);
	src_len = ft_strlen(src);
	if (!dst || size == 0)
		return (src_len);
	i = 0;
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

/*
 * ft_strlcat - Concatenate strings with size limit
 */
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	if (!dst || !src)
		return (0);
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dst_len >= size)
		return (size + src_len);
	i = 0;
	while (dst_len + i < size - 1 && src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return dst_len + src_len;
}

/*
 * ft_atoi - Convert string to integer
 */
int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	if (!nptr)
		return 0;
	// Skip whitespace
	while (*nptr == ' ' || *nptr == '\t' || *nptr == '\n' || *nptr == '\v'
		|| *nptr == '\f' || *nptr == '\r')
		nptr++;
	// Handle sign
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
	{
		nptr++;
	}
	// Convert digits
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		nptr++;
	}
	return result * sign;
}

/*
 * ft_itoa - Convert integer to string
 */
char	*ft_itoa(int n)
{
	char	*result;
	int		len;
	int		temp;
	int		i;

	len = 0;
	temp = n;
	// Handle special case
	if (n == 0)
		return ft_strdup("0");
	// Calculate length
	if (n < 0)
	{
		len = 1; // for minus sign
		temp = -temp;
		if (n == -2147483648) // Handle INT_MIN
			return ft_strdup("-2147483648");
	}
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	// Allocate memory
	result = malloc(len + 1);
	if (!result)
		return NULL;
	result[len] = '\0';
	// Fill string backwards
	temp = n;
	if (temp < 0)
		temp = -temp;
	i = len - 1;
	while (temp > 0)
	{
		result[i] = (temp % 10) + '0';
		temp /= 10;
		i--;
	}
	if (n < 0)
		result[0] = '-';
	return result;
}

/*
 * ft_strnstr - Locate substring in string with length limit
 */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t needle_len;
	size_t i;
	size_t j;

	if (!haystack || !needle)
		return NULL;

	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return (char *)haystack;

	i = 0;
	while (i < len && haystack[i])
	{
		j = 0;
		while (j < needle_len && i + j < len && haystack[i + j] && haystack[i
			+ j] == needle[j])
			j++;

		if (j == needle_len)
			return (char *)(haystack + i);

		i++;
	}

	return NULL;
}