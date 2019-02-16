/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:48 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 16:17:10 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "common.h"

t_ecode	ping_op_handler(t_message *msg, t_env *env)
{
	(void)msg;
	message_send(E_MESSAGE_OK, NULL, 0, env->csock);
	env->log(env, "pong\n");
	return (E_ERR_OK);
}

t_bool	exec_cmd_ping(char *const *args, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	(void)args;
	if ((err = message_send(E_MESSAGE_PING, NULL, 0, e->csock)))
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return (false);
	}
	if ((err = message_receive(&msg, e->csock)))
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return (false);
	}
	ret = msg->hd.op == E_MESSAGE_OK;
	message_destroy(msg);
	return (ret);
}
