/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 14:19:45 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 16:52:39 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include <limits.h>

t_ecode			pwd_op_handler(t_message *msg, t_env *env)
{
	size_t	offset;
	size_t	len;

	(void)msg;
	offset = ft_strlen(env->root_path);
	len = ft_strlen(env->cwd_path);
	if (len <= offset)
		return (message_send(E_MESSAGE_OK, "/", 2, env->csock));
	else
		return (message_send(E_MESSAGE_OK, env->cwd_path + offset,
					len - offset + 1, env->csock));
}

t_bool			exec_cmd_lpwd(char *const *args, char const **reason, t_env *e)
{
	(void)args;
	(void)reason;
	fprintf(stdout, "%s\n", e->cwd_path);
	return (true);
}

static t_ecode	handle_response(t_message *msg, t_env *e)
{
	if (msg->hd.op == E_MESSAGE_OK)
	{
		if (msg->hd.size > 0 && msg->hd.size <= INT_MAX)
		{
			e->log(e, "%.*s", (int)msg->hd.size - 1, msg->payload);
			return (E_ERR_OK);
		}
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0 && msg->hd.size <= INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
		return (E_ERR_SERVER);
	}
	else
		return (E_ERR_UNEXPECTED_OP);
}

t_bool			exec_cmd_pwd(char *const *args, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	(void)args;
	if (!(err = message_send(E_MESSAGE_PWD, NULL, 0, e->csock)))
		err = message_receive(&msg, e->csock);
	if (err)
	{
		e->should_quit = true;
		*reason = error_get_string(err);
		return (false);
	}
	if ((err = handle_response(msg, e)))
		*reason = error_get_string(err);
	ret = (err == E_ERR_OK);
	message_destroy(msg);
	return (ret);
}
