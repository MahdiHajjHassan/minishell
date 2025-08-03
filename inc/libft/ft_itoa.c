/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahajj-h <mahajj-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 19:46:59 by mahajj-h          #+#    #+#             */
/*   Updated: 2024/06/17 12:51:07 by mahajj-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbr_len(int nbr)
{
	int	len;

	len = 1;
	if (nbr < 0)
	{
		len++;
		if (nbr != -2147483648)
			nbr = -nbr;
		else
			nbr = -(nbr + 1);
	}
	while (nbr > 9)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

static void	handle_negative(int *nbr, int *is_negative)
{
	if (*nbr < 0)
	{
		*is_negative = 1;
		if (*nbr != -2147483648)
			*nbr = -*nbr;
	}
}

static char	*convert_to_string(int nbr, int is_negative, int len)
{
	char	*res;
	int		i;

	res = (char *)malloc((len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	i = len - 1;
	while (nbr > 0)
	{
		res[i--] = (nbr % 10) + '0';
		nbr /= 10;
	}
	if (is_negative)
		res[0] = '-';
	res[len] = '\0';
	return (res);
}

char	*ft_itoa(int nbr)
{
	int		len;
	int		is_negative;
	char	*res;

	is_negative = 0;
	len = nbr_len(nbr);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	if (nbr == 0)
	{
		res[0] = '0';
		res[1] = '\0';
		return (res);
	}
	if (nbr == -2147483648)
		return (ft_strdup("-2147483648"));
	handle_negative(&nbr, &is_negative);
	if (nbr == 0)
	{
		res[0] = '0';
		res[1] = '\0';
		return (res);
	}
	return (convert_to_string(nbr, is_negative, len));
}
