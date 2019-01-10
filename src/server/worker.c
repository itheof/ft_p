#include "server.h"

/*
** in the future, if forking is required in the worker process, remember:
** a custom handler is set for sig
*/

typedef	t_ecode	(*t_op_handler)(t_env *env);

t_ecode	ping_op_handler(t_env *env)
{
	message_send(E_MESSAGE_OK, NULL, 0, env->csock);
	print_header(env->pid);
	printf("pong\n");
}

t_ecode	default_op_handler(t_env *env)
{
	env->should_quit = true;
	return (E_ERR_UNIMPLEMENTED_OP);
}

static const t_op_handler	g_op_handlers[E_MESSAGE_MAX] = {
	[E_MESSAGE_PING] = ping_op_handler,
/*	[E_MESSAGE_LS] =,
	[E_MESSAGE_CD] =,
	[E_MESSAGE_GET] =,
	[E_MESSAGE_PUT] =,
	[E_MESSAGE_PWD] =,*/
};

static void	worker_init(t_env *env, int cs)
{
	env->should_quit = false;
	env->pid = getpid();
	env->csock = cs;
}

void	connection_worker(int cs)
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
		err = handler(&env);
		message_destroy(msg);
	}
	if (err)
	{
		print_header(env.pid);
		printf("%s\n", error_get_string(err));
	}
	close(env.csock);
	exit(err);
}
