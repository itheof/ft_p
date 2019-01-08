/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 10:52:24 by tvallee           #+#    #+#             */
/*   Updated: 2014/11/06 19:17:21 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*r;
	size_t	i;

	r = (s) ? malloc(sizeof(char) * (1 + ft_strlen(s))) : NULL;
	i = 0;
	if (s && f && r)
	{
		while (i < ft_strlen(s))
		{
			r[i] = f(s[i]);
			i++;
		}
		r[i] = 0;
	}
	return (r);
}
