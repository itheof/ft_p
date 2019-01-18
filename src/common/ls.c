#include "common.h"
#include <dirent.h>

t_ecode	ls_op_handler(t_message *msg, t_env *env)
{
	t_ecode			err;
	DIR				*dirp;
	struct dirent	*dp;

	dirp = opendir(".");
	if (dirp == NULL)
	{
		env->log(env, "ls: opendir() failed: %s", strerror(errno));
		return (message_send(E_MESSAGE_ERR, strerror(errno), ft_strlen(strerror(errno)), env->csock));
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue ;
#if defined(__APPLE__)
		if ((err = message_send(E_MESSAGE_OK, dp->d_name, dp->d_namlen, env->csock)))
#else
		if ((err = message_send(E_MESSAGE_OK, dp->d_name, ft_strlen(dp->d_name), env->csock)))
#endif
		{
			closedir(dirp);
			env->should_quit = true;
			return (err);
		}
	}
	closedir(dirp);
	return (message_send(E_MESSAGE_OK, NULL, 0, env->csock));
}

t_bool	exec_cmd_ls(char * const *args, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	(void)args;
	if ((err = message_send(E_MESSAGE_LS, NULL, 0, e->csock)))
		return (command_abort(err, reason, &e->should_quit));
	while (!(err = message_receive(&msg, e->csock))
			&& msg->hd.op == E_MESSAGE_OK && msg->hd.size != 0)
	{
		printf("%.*s\n", msg->hd.size, msg->payload);
		message_destroy(msg);
	}
	if (err)
		return (command_abort(err, reason, &e->should_quit));
	if (msg->hd.op == E_MESSAGE_OK)
		ret = true;
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0)
			e->log(e, "server: %.*s", msg->hd.size - 1, msg->payload);
		*reason = error_get_string(E_ERR_SERVER);
		ret = false;
	}
	else
	{
		*reason = error_get_string(E_ERR_UNEXPECTED_OP);
		ret = false;
	}
	message_destroy(msg);
	return (ret);

}
