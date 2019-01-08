/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strnstr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 14:04:56 by tvallee           #+#    #+#             */
/*   Updated: 2014/11/06 18:25:10 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	int		i;

	if (s2[0] == 0)
		return ((char *)s1);
	while (*s1 && n)
	{
		while (*s1 != s2[0] && *s1 && n)
		{
			n--;
			s1++;
		}
		i = 0;
		while (s1[i] && s2[i] && s1[i] == s2[i] && n)
		{
			i++;
			n--;
		}
		if (s2[i] == 0)
			return ((char *)s1);
	}
	return (NULL);
}
