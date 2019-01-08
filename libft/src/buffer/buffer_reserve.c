/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_reserve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 17:05:42 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/12 17:56:42 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdbool.h"
#include "libft/buffer.h"
#include "libft.h"

t_bool	buffer_reserve(t_buffer *buf, size_t avail)
{
	char	*tmp;
	size_t	target_len;
	size_t	target_size;

	target_len = buf->len + avail;
	if (target_len + 1 <= buf->size)
		return (true);
	else
	{
		target_size = align_size(target_len + 1, BUFFER_BASE_SIZE);
		tmp = malloc(target_size);
		if (tmp == NULL)
			return (false);
		ft_strcpy(tmp, buf->str);
		free(buf->str);
		buf->str = tmp;
		buf->size = target_size;
		return (true);
	}
}
