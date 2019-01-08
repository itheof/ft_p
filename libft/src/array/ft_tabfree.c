/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freetab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/12/30 10:50:57 by tvallee           #+#    #+#             */
/*   Updated: 2015/01/13 17:13:14 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_freetab(void **tab)
{
	int i;

	if (tab)
	{
		i = 0;
		while (tab[i] != 0)
			free(tab[i++]);
		free(tab);
	}
	return (0);
}
