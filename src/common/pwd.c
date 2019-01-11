#include "common.h"

t_ecode	pwd_op_handler(t_message *msg, t_env *env)
{
	char	*pwd;
	int		saved_errno;
	t_ecode	e;
	size_t	len;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		saved_errno = errno;
		env->log(env, "pwd: getcwd() call failed: %s", strerror(errno));
		return (message_send_unknown_err(env->csock, saved_errno));
	}
	len = ft_strlen(pwd);
	e = message_send(E_MESSAGE_OK, pwd, len + 1, env->csock);
	free(pwd);
	return (e);
}

t_bool	exec_cmd_pwd(char * const *args, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	(void)args;
	if ((err = message_send(E_MESSAGE_PWD, NULL, 0, e->csock)))
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return false;
	}
	if ((err = message_receive(&msg, e->csock)))
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return false;
	}
	ret = false;
	if (msg->hd.op == E_MESSAGE_OK)
	{
		if (msg->hd.size > 0)
		{
			e->log(e, "%*s", msg->hd.size - 1, msg->payload);
			ret = true;
		}
		else
			*reason = error_get_string(E_ERR_INVALID_PAYLOAD);
	}
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
