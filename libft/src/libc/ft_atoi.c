/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 11:11:36 by tvallee           #+#    #+#             */
/*   Updated: 2014/12/10 15:12:56 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *str)
{
	int		i;
	int		minus;

	i = 0;
	minus = 1;
	while ((*str < 14 && *str > 8) || *str == ' ')
		str++;
	if (*str == '-')
		minus = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str > 47 && *str < 58)
		i = i * 10 + *str++ - '0';
	return (i * minus);
}
