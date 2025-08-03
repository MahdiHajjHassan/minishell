/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 14:02:55 by mahajj-h          #+#    #+#             */
/*   Updated: 2024/06/17 10:45:07 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	target;

	target = (unsigned char)c;
	if (target == '\0')
	{
		while (*s != '\0')
			s++;
		return ((char *)s);
	}
	while (*s != '\0')
	{
		if (*s == target)
			return ((char *)s);
		s++;
	}
	return (NULL);
}
