/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 17:30:28 by tvallee           #+#    #+#             */
/*   Updated: 2014/11/05 20:25:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*lst;

	if (alst != NULL)
	{
		if (*alst != NULL)
		{
			lst = *alst;
			del(lst->content, lst->content_size);
			free(*alst);
		}
		*alst = NULL;
	}
}
