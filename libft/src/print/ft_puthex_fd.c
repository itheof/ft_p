/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 12:59:05 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/13 16:37:04 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/print.h"

void		ft_puthex_fd(size_t n, int fd)
{
	const char	*map = "0123456789ABCDEF";
	char		buf[(sizeof(size_t) * 2) + 3];
	size_t		i;
	unsigned	half_byte;

	buf[sizeof(buf) - 1] = '\0';
	i = sizeof(buf) - 2;
	while (n)
	{
		half_byte = n & 0xF;
		buf[i] = map[half_byte];
		n >>= 4;
		i--;
	}
	if (i == sizeof(buf) - 2)
		buf[i--] = '0';
	buf[i--] = 'x';
	buf[i] = '0';
	ft_putstr_fd(buf + i, fd);
}
