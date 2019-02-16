#include "common.h"
#include <dirent.h>
#include <limits.h>

t_ecode	ls_op_handler(t_message *msg, t_env *env)
{
	t_ecode			err;
	DIR				*dirp;
	struct dirent	*dp;

	(void)msg;
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
		if ((err = message_send(E_MESSAGE_OK, dp->d_name, ft_strlen(dp->d_name), env->csock)))
		{
			closedir(dirp);
			env->should_quit = true;
			return (err);
		}
	}
	closedir(dirp);
	return (message_send(E_MESSAGE_OK, NULL, 0, env->csock));
}

t_bool	exec_cmd_lls(char * const *args, char const **reason, t_env *e)
{
	DIR				*dirp;
	struct dirent	*dp;

	(void)args;
	dirp = opendir(".");
	if (dirp == NULL)
	{
		e->log(e, "ls: opendir() failed: %s", strerror(errno));
		*reason = error_get_string(E_ERR_OPENDIR);
		return (false);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue ;
		fprintf(stdout, "%s\n", dp->d_name);
	}
	closedir(dirp);
	return (true);
}

t_bool	exec_cmd_ls(char * const *args, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	(void)args;
	if ((err = message_send(E_MESSAGE_LS, NULL, 0, e->csock)))
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return (false);
	}
	while (!(err = message_receive(&msg, e->csock))
			&& msg->hd.op == E_MESSAGE_OK && msg->hd.size != 0)
	{
		printf("%.*s\n", (unsigned)msg->hd.size, msg->payload);
		message_destroy(msg);
	}
	if (err)
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return (false);
	}
	if (msg->hd.op == E_MESSAGE_OK)
		ret = true;
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0 && msg->hd.size < INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
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
