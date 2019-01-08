/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lsttotab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/13 16:55:44 by tvallee           #+#    #+#             */
/*   Updated: 2015/01/13 17:10:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	**ft_lsttotab(t_list *list, int destroy)
{
	void	**tab;
	int		len;
	int		i;
	t_list	bu;

	i = 0;
	len = ft_lstlen(list);
	tab = malloc(sizeof(*tab) * (len + 1));
	while (list)
	{
		tab[i] = list->content;
		bu = *list;
		if (destroy)
			free(list);
		list = bu.next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
