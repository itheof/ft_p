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

static t_ecode	fail_with_fs_race_condition(t_env *env)
{
	env->log(env, "filesystem race condition detected: bailing");
	env->should_quit = true;
	return (message_send(E_MESSAGE_ERR, NULL, 0, env->csock));
}

/*
 * NOTE: Do not send the error reason to the client: this would lead to
 * information disclosure when ie attempting to chdir to a not-dir-path outside
 * the root_path.
 * Reporting the error cause would require a long spaghetti implementation for
 * preventing root_dir escaping
 *
 * This implementation has tons of possible error conditions, BUT is simple and
 * secure.
 * Calls to getcwd after chdir gives us a cheap realpath()
 *
 * It does not support logical paths cd
 */
t_ecode	cd_op_handler(t_message *msg, t_env *env)
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
		env->log(env, "cd: chdir() call failed: %s %s", strerror(errno), msg->payload);
		return (message_send(E_MESSAGE_ERR, NULL, 0, env->csock));
	}
	if (!(newp = getcwd(NULL, 0)))
	{
		env->log(env, "cd: getcwd() call failed: %s", strerror(errno));
		return (fail_with_fs_race_condition(env));
	}

	if (has_basepath(env->root_path, newp)) // SUCCESS
	{
		env->log(env, "cd: %s -> %s", env->cwd_path, newp);
		free(env->cwd_path);
		env->cwd_path = newp;
		return (message_send(E_MESSAGE_OK, NULL, 0, env->csock));
	}

	if (chdir(env->cwd_path) == 0) // FAILURE BUT OK
	{
		env->log(env, "cd: recovered from illegal chdir: %s "
				"escapes root_path %s", newp, env->root_path);
		free(newp);
		return (message_send(E_MESSAGE_ERR, NULL, 0, env->csock));
	}
	else
	{
		env->log(env, "cd: chdir() call failed: %s %s", strerror(errno), env->cwd_path);
		return (fail_with_fs_race_condition(env));
	}
}

t_bool	exec_cmd_lcd(char *const *args, char const **reason, t_env *e)
{
	if (chdir(args[1]) != 0)
	{
		perror("chdir");
		*reason = error_get_string(E_ERR_CHDIR);
		return (false);
	}
	free(e->cwd_path);
	if (!(e->cwd_path = getcwd(NULL, 0)))
	{
		perror("getcwd");
		*reason = error_get_string(E_ERR_GETCWD);
		e->should_quit = true;
		// XXX using e->cwd_path is now UB
		return (false);
	}
	return (true);
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
		if (msg->hd.size > 0 && msg->hd.size <= INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
		*reason = error_get_string(E_ERR_SERVER);
	}
	else
		*reason = error_get_string(E_ERR_UNEXPECTED_OP);
	message_destroy(msg);
	return (ret);
}
