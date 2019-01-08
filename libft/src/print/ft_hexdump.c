/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 17:10:38 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/08 17:28:45 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/print.h"
#include "libft/stdbool.h"

static void	put_byte(char byte)
{
	const char	*map = "0123456789abcdef";

	ft_putchar_fd(map[(byte & 0xF0) >> 4], 2);
	ft_putchar_fd(map[byte & 0xF], 2);
}

void		ft_hexdump(const char *addr, size_t nbytes)
{
	size_t		i;

	i = 0;
	if (nbytes == 0)
		return ;
	while (true)
	{
		put_byte(*addr++);
		nbytes--;
		i++;
		if (nbytes > 0)
		{
			if (i == 16)
			{
				i = 0;
				ft_putchar_fd('\n', 2);
			}
			else if (!(i & 0x1))
				ft_putchar_fd(' ', 2);
		}
		else
			break ;
	}
	ft_putchar_fd('\n', 2);
}
