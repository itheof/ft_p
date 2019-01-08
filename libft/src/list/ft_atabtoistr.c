/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atabtoistr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/13 17:31:06 by tvallee           #+#    #+#             */
/*   Updated: 2015/01/13 17:58:07 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		*ft_atabtoistr(char **tab)
{
	int len;
	int i;
	int *str;

	len = 0;
	while (tab[len] != NULL)
		len++;
	str = malloc(sizeof(*str) * (len + 2));
	i = 0;
	str[0] = len + 1;
	while (tab[i] != NULL)
	{
		str[i + 1] = ft_atoi(tab[i]);
		i++;
	}
	return (str);
}
