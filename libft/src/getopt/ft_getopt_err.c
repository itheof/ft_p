/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_err.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 22:38:09 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/20 11:47:30 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/print.h"
#include "libft/getopt.h"

void	ft_getopt_err(const char *name, const char *reason, char invalid)
{
	ft_putstr_fd(name, 2);
	ft_putstr_fd(reason, 2);
	ft_putchar_fd(invalid, 2);
	ft_putstr_fd("'\n", 2);
}
