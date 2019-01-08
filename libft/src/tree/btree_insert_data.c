/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_insert_data.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 12:40:10 by tvallee           #+#    #+#             */
/*   Updated: 2015/10/08 13:45:06 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *,
			void *))
{
	if (*root)
	{
		if (cmpf(item, (*root)->item) < 0)
			btree_insert_data((*root)->left, item, cmpf);
		else
			btree_insert_data((*root)->right, item, cmpf);
	}
	else
		*root = btree_create_node(item);
}
