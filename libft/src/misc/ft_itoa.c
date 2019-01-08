/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 15:02:31 by tvallee           #+#    #+#             */
/*   Updated: 2014/11/07 01:05:59 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	int		len;
	int		nb;
	char	*str;

	nb = n;
	len = (n < 0) ? 2 : 1;
	while (nb / 10)
	{
		nb = nb / 10;
		len++;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (str)
	{
		str[len] = 0;
		str[0] = 65;
		if (n < 0)
			str[0] = '-';
		while (len && !(len == 1 && str[0] == '-'))
		{
			str[--len] = (n < 0) ? -1 * (n % 10) + '0' : n % 10 + '0';
			n = n / 10;
		}
	}
	return (str);
}
