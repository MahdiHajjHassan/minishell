/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 23:51:06 by mahajj-h          #+#    #+#             */
/*   Updated: 2024/06/17 11:39:45 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	ft_putendl_fd(char *s, int fd)
{
	size_t	len;
	char	newline;

	if (s)
	{
		len = 0;
		while (s[len])
			len++;
		write(fd, s, len);
		newline = '\n';
		write(fd, &newline, 1);
	}
}
