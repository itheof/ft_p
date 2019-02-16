/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 16:58:54 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 17:37:07 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include <limits.h>

static t_bool	has_basepath(char const *basepath, char const *subpath)
{
	size_t	n;

	n = 0;
	printf("%s\n", basepath);
	while (basepath[n] != 0 && basepath[n] == subpath[n])
		n++;
	if (!ft_strcmp(basepath, "/"))
		return (true);
	if (basepath[n] == 0 && (subpath[n] == '/' || subpath[n] == 0))
		return (true);
	if (basepath[n] == '/' && subpath[n] == 0 && basepath[n + 1] == 0)
		return (true);
	return (false);
}

/*
** NOTE: Do not send the error reason to the client: this would lead to
** information disclosure when ie attempting to chdir to a not-dir-path outside
** the root_path.
** Reporting the error cause would require a long spaghetti implementation for
** preventing root_dir escaping
**
** This implementation has tons of possible error conditions, BUT is simple and
** secure.
** Calls to getcwd after chdir gives us a cheap realpath()
**
** It does not support logical paths cd
*/

t_ecode			cd_op_handler2(char *newp, t_env *env)
{
	if (has_basepath(env->root_path, newp))
	{
		env->log(env, "cd: %s -> %s", env->cwd_path, newp);
		free(env->cwd_path);
		env->cwd_path = newp;
		return (message_send(E_MESSAGE_OK, NULL, 0, env->csock));
	}
	if (chdir(env->cwd_path) == 0)
	{
		env->log(env, "cd: recovered from illegal chdir: %s "
				"escapes root_path %s", newp, env->root_path);
		free(newp);
		return (message_send(E_MESSAGE_ERR, NULL, 0, env->csock));
	}
	else
	{
		env->log(env, "cd: chdir() call failed: %s %s",
				strerror(errno), env->cwd_path);
		env->log(env, "filesystem race condition detected: bailing");
		env->should_quit = true;
		return (message_send(E_MESSAGE_ERR, NULL, 0, env->csock));
	}
}

t_ecode			cd_op_handler(t_message *msg, t_env *env)
{
	char	*newp;

	if (!msg->hd.size)
	{
		env->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	msg->payload[msg->hd.size - 1] = '\0';
	if (chdir(msg->payload) != 0)
	{
		env->log(env, "cd: chdir() call failed: %s %s",
				strerror(errno), msg->payload);
		return (message_send(E_MESSAGE_ERR, NULL, 0, env->csock));
	}
	if (!(newp = getcwd(NULL, 0)))
	{
		env->log(env, "cd: getcwd() call failed: %s", strerror(errno));
		env->log(env, "filesystem race condition detected: bailing");
		env->should_quit = true;
		return (message_send(E_MESSAGE_ERR, NULL, 0, env->csock));
	}
	return (cd_op_handler2(newp, env));
}

t_bool			handle_response(t_message *msg, char const **reason, t_env *e)
{
	t_bool	ret;

	ret = false;
	if (msg->hd.op == E_MESSAGE_OK)
		ret = true;
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0 && msg->hd.size <= INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
		*reason = error_get_string(E_ERR_SERVER);
	}
	else
		*reason = error_get_string(E_ERR_UNEXPECTED_OP);
	message_destroy(msg);
	return (ret);
}

t_bool			exec_cmd_cd(char *const *args, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;

	(void)args;
	if ((err = message_send(E_MESSAGE_CD,
					args[1], ft_strlen(args[1]) + 1, e->csock)))
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
	return (handle_response(msg, reason, e));
}
