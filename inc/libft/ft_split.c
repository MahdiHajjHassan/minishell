/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 19:15:50 by mahajj-h          #+#    #+#             */
/*   Updated: 2024/06/17 12:20:00 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c)
		{
			count++;
			while (str[i] != c && str[i] != '\0')
				i++;
		}
		if (str[i] != '\0')
			i++;
	}
	return (count);
}

static void	ft_strncpy(char *str, char const *src, int n)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		str[i] = (char)src[i];
		i++;
	}
	str[i] = '\0';
}

static void	helper(char c, char const *s, char **strs, int *k)
{
	int	j;
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			j = 0;
			while (s[i + j] != c && s[i + j] != '\0')
				j++;
			strs[*k] = malloc((j + 1) * sizeof(char));
			ft_strncpy(strs[(*k)++], (char *)s + i, j);
			i += j;
		}
		else
			i++;
	}
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		k;

	strs = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	k = 0;
	if (strs == NULL)
		return (NULL);
	helper(c, s, strs, &k);
	strs[k] = NULL;
	return (strs);
}
