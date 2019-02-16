/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 17:47:35 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 17:50:08 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/*
** in the future, if forking is required in the worker process, remember:
** a custom handler is set for sig
*/

t_ecode		default_op_handler(t_message *msg, t_env *env)
{
	(void)msg;
	env->should_quit = true;
	return (E_ERR_UNIMPLEMENTED_OP);
}

static int	log_with_pid(t_env const *env, char const *format, ...)
{
	va_list args;
	int		ret;
	pid_t	tmp;
	int		i;

	tmp = env->pid;
	i = sizeof("()") - sizeof("");
	while (tmp > 0)
	{
		tmp /= 10;
		i++;
	}
	assert(LOG_PADDING >= i);
	ret = printf("worker(%d)%*c: ", env->pid, LOG_PADDING - i, ' ');
	va_start(args, format);
	ret += vprintf(format, args);
	ret += printf("\n");
	va_end(args);
	return (ret);
}

static const t_op_handler	g_op_handlers[E_MESSAGE_MAX] = {
	[E_MESSAGE_PING] = ping_op_handler,
	[E_MESSAGE_CD] = cd_op_handler,
	[E_MESSAGE_LS] = ls_op_handler,
	[E_MESSAGE_GET] = get_op_handler,
	[E_MESSAGE_PUT] = put_op_handler,
	[E_MESSAGE_PWD] = pwd_op_handler,
};

static void	worker_init(t_env *env, int cs)
{
	env->root_path = getenv("PWD");
	env->cwd_path = ft_strdup(env->root_path);
	assert(env->root_path);
	env->log = log_with_pid;
	env->should_quit = false;
	env->pid = getpid();
	env->csock = cs;
}

void		connection_worker(int cs)
{
	t_op_handler	handler;
	t_message		*msg;
	t_ecode			err;
	t_env			env;

	worker_init(&env, cs);
	while (!env.should_quit && !(err = message_receive(&msg, env.csock)))
	{
		if (!(handler = g_op_handlers[msg->hd.op]))
			handler = default_op_handler;
		err = handler(msg, &env);
		message_destroy(msg);
	}
	if (err)
		env.log(&env, "%s\n", error_get_string(err));
	close(env.csock);
	exit(err);
}
