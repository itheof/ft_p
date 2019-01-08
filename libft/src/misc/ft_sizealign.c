/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_align.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 15:57:26 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/13 09:42:23 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Alignment must be a power of 2 && >=2
*/

size_t	align_size(size_t size, size_t alignment)
{
	size_t	truncated;

	truncated = size & (~(alignment - 1));
	if (truncated < size)
		return (truncated + alignment);
	else
		return (truncated);
}
