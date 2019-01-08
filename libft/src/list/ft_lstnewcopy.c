/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnewcopy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/15 16:50:24 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/20 10:08:58 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnewcopy(void *data, size_t data_size)
{
	t_list	*new;

	new = malloc(sizeof(*new));
	if (data)
	{
		new->content = data;
		new->content_size = data_size;
		new->next = NULL;
	}
	return (new);
}
