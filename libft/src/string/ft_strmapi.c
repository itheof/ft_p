/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 11:04:18 by tvallee           #+#    #+#             */
/*   Updated: 2014/11/06 19:18:08 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*r;
	unsigned int	i;

	i = 0;
	r = (s) ? malloc(sizeof(char) * (1 + ft_strlen(s))) : NULL;
	if (f && s && r)
	{
		while (i < ft_strlen(s))
		{
			r[i] = f(i, s[i]);
			i++;
		}
	}
	return (r);
}
