/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 17:44:00 by tvallee           #+#    #+#             */
/*   Updated: 2014/11/05 18:00:25 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*current;

	if (alst)
	{
		current = *alst;
		if (current)
		{
			ft_lstdel(&current->next, del);
			del(current->content, current->content_size);
			free(current);
		}
		*alst = NULL;
	}
}
