/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 17:03:06 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/12 17:53:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdbool.h"
#include "libft/buffer.h"

t_bool	buffer_init_size(t_buffer *buf, size_t size)
{
	if ((buf->str = malloc(size)) == NULL)
		return (false);
	buf->size = size;
	buf->len = 0;
	*buf->str = '\0';
	return (true);
}

t_bool	buffer_init(t_buffer *buf)
{
	return (buffer_init_size(buf, BUFFER_BASE_SIZE));
}

t_bool	buffer_init_with(t_buffer *buf, const char *str)
{
	if (buffer_init(buf))
		return (buffer_cat(buf, str));
	else
		return (false);
}
