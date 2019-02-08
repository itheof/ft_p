#include "common.h"
#include <limits.h>

t_ecode	pwd_op_handler(t_message *msg, t_env *env)
{
	size_t	offset;
	size_t	len;

	(void)msg;
	offset = ft_strlen(env->root_path);
	len = ft_strlen(env->cwd_path);
	if (len <= offset)
		return (message_send(E_MESSAGE_OK, "/", 2, env->csock));
	else
		return (message_send(E_MESSAGE_OK, env->cwd_path + offset, len - offset + 1, env->csock));
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
		if (msg->hd.size > 0 && msg->hd.size <= INT_MAX)
		{
			e->log(e, "%.*s", (int)msg->hd.size - 1, msg->payload);
			ret = true;
		}
		else
			*reason = error_get_string(E_ERR_INVALID_PAYLOAD);
	}
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
