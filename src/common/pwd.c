#include "common.h"

t_ecode	pwd_op_handler(t_message *msg, t_env *env)
{
	message_send(E_MESSAGE_OK, NULL, 0, env->csock);
	printf("coucou\n");
	return (E_ERR_OK);
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
	ret = msg->hd.op == E_MESSAGE_OK;
	message_destroy(msg);
	return (ret);
}
