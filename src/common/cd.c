#include "common.h"

t_ecode	cd_op_handler(t_message *msg, t_env *env)
{
	int		saved_errno;
	t_ecode	e;

	if (!msg->hd.size)
	{
		env->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	msg->payload[msg->hd.size - 1] = '\0';
	// TODO: sanitize path
	if (chdir(msg->payload) != 0)
	{
		saved_errno = errno;
		env->log(env, "cd: chdir() call failed: %s", strerror(errno));
		return (message_send_unknown_err(env->csock, saved_errno));
	}
	e = message_send(E_MESSAGE_OK, NULL, 0, env->csock);
	return (e);
}

t_bool	exec_cmd_cd(char * const *args, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	(void)args;
	if ((err = message_send(E_MESSAGE_CD, args[1], ft_strlen(args[1]) + 1, e->csock)))
		return (command_abort(err, reason, &e->should_quit));
	if ((err = message_receive(&msg, e->csock)))
		return (command_abort(err, reason, &e->should_quit));
	ret = false;
	if (msg->hd.op == E_MESSAGE_OK)
		ret = true;
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0)
			e->log(e, "server: %*s", msg->hd.size - 1, msg->payload);
		*reason = error_get_string(E_ERR_SERVER);
	}
	else
		*reason = error_get_string(E_ERR_UNEXPECTED_OP);
	message_destroy(msg);
	return (ret);
}
