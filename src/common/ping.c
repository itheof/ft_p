/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:48 by tvallee           #+#    #+#             */
/*   Updated: 2018/12/17 15:57:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include "common.h"

t_bool	exec_cmd_ping(char * const *args, char const **reason, t_cenv *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	(void)args;
	if ((err = message_send(E_MESSAGE_PING, NULL, 0, e->sock)))
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return false;
	}
	if ((err = message_receive(&msg, e->sock)))
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return false;
	}
	ret = msg->hd.op == E_MESSAGE_OK;
	message_destroy(msg);
	return (ret);
}
