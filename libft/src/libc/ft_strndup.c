/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 22:21:56 by tvallee           #+#    #+#             */
/*   Updated: 2015/05/03 22:22:07 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char const *str, size_t n)
{
	int		i;
	char	*new;

	i = 0;
	new = malloc(sizeof(*new) * (n + 1));
	while (n-- && *str)
	{
		new[i++] = *str++;
	}
	new[i] = 0;
	return (new);
}
