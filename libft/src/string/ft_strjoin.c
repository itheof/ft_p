/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 22:20:05 by tvallee           #+#    #+#             */
/*   Updated: 2016/09/19 15:46:08 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*r;
	int		i;
	int		j;

	r = NULL;
	if (!(i = 0) && s1 && s2)
	{
		r = ft_memalloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
		if (r && (j = 0) == 0)
		{
			while (s1[i])
			{
				r[i] = s1[i];
				i++;
			}
			while (s2[j])
			{
				r[i + j] = s2[j];
				j++;
			}
			r[i + j] = 0;
		}
	}
	return (r);
}
