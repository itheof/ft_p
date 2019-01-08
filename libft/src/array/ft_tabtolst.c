/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabtolst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/05 21:01:48 by tvallee           #+#    #+#             */
/*   Updated: 2015/02/05 21:32:16 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_tabtolst(void **tab)
{
	t_list	*current;
	int		i;

	i = 0;
	current = NULL;
	if (!tab)
		return (NULL);
	while (tab[i])
		ft_lstpushback(&current, ft_lstnewcopy(tab[i++], 0));
	free(tab);
	return (current);
}
