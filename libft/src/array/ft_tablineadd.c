/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tablineadd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/30 13:59:10 by tvallee           #+#    #+#             */
/*   Updated: 2015/01/30 18:05:21 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	**ft_tablineadd(void *line, void **tab)
{
	void	**newtab;
	size_t	i;
	size_t	l;

	if (!(newtab = NULL) && tab && line)
	{
		l = ft_tablen(tab);
		newtab = malloc(sizeof(*newtab) * (l + 2));
		i = 0;
		while ((newtab[i] = tab[i]))
			i++;
		free(tab);
		newtab[i] = line;
		newtab[i + 1] = NULL;
		return (newtab);
	}
	else if (tab)
		newtab = tab;
	else if (line)
	{
		newtab = ft_memalloc(sizeof(*newtab) * 2);
		newtab[0] = line;
		newtab[1] = NULL;
	}
	return (newtab);
}
