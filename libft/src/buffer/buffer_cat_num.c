/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_cat_num.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/26 15:02:17 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/26 15:05:52 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft/buffer.h"

t_bool			buffer_cat_num(t_buffer *buf, size_t n)
{
	char	*str;
	t_bool	success;

	str = ft_itoa(n);
	if (str != NULL)
	{
		success = buffer_cat(buf, str);
		free(str);
		return (success);
	}
	else
		return (false);
}
