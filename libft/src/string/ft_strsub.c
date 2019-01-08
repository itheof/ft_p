/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 11:30:43 by tvallee           #+#    #+#             */
/*   Updated: 2014/12/24 19:15:29 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*r;

	if (s)
	{
		i = 0;
		r = malloc(sizeof(*r) * (len + 1));
		if (r)
		{
			while (i < len)
			{
				r[i] = s[start + i];
				i++;
			}
			r[i] = 0;
		}
		return (r);
	}
	return (NULL);
}
