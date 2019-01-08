/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 16:27:47 by tvallee           #+#    #+#             */
/*   Updated: 2015/01/15 16:48:02 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *data, size_t data_size)
{
	t_list	*new;

	new = ft_memalloc(sizeof(*new));
	if (data)
	{
		new->content = malloc(data_size);
		new->next = NULL;
		ft_memcpy(new->content, (void*)data, data_size);
	}
	new->content_size = (data) ? data_size : 0;
	return (new);
}
