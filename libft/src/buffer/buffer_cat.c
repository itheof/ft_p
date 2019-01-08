/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_cat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 17:04:04 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/12 17:55:08 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdbool.h"
#include "libft/buffer.h"
#include "libft/libc.h"

t_bool	buffer_ncat(t_buffer *buf, const char *str, size_t n)
{
	if (!buffer_reserve(buf, n))
		return (false);
	else
	{
		ft_strncat(buf->str + buf->len, str, n);
		buf->len += ft_strlen(buf->str + buf->len);
		return (true);
	}
}

t_bool	buffer_cat(t_buffer *buf, const char *str)
{
	return (buffer_ncat(buf, str, ft_strlen(str)));
}
