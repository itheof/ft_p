/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:25:55 by tvallee           #+#    #+#             */
/*   Updated: 2018/12/17 14:26:01 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

t_ecode	message_send_unknown_err(int sock, int errnum)
{
	size_t		len;
	char 		*err;

	err = strerror(errnum);
	len = ft_strlen(err);
	return (message_send(E_MESSAGE_ERR, err, len + 1, sock));
}

int		ft_atoi_sane(char const *s, t_bool *sane)
{
	char	*tmp;
	int		i;

	i = ft_atoi(s);
	*sane = false;
	tmp = ft_itoa(i);
	if (tmp != NULL&& !ft_strcmp(tmp, s))
		*sane = true;
	free(tmp);
	return (i);
}
