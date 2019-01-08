/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 23:38:22 by tvallee           #+#    #+#             */
/*   Updated: 2014/11/06 17:35:27 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*str;

	str = malloc((size + 1) * sizeof(char));
	if (str)
	{
		while (size != 0)
		{
			str[size] = 0;
			size--;
		}
		str[size] = 0;
	}
	return (str);
}
