/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 17:05:36 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 17:32:42 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	g_not_basename_err[] = "get: file must be a single path "
"component, relative to the current directory";

static const char	g_not_regular_err[] = "get: file is not a regular file";

static t_bool	is_regular_file_or_not_exist(char const *filename, t_env *e)
{
	DIR		*dirp;
	struct dirent	*dp;
	t_bool	ret;

	dirp = opendir(".");
	if (dirp == NULL)
	{
		e->log(e, "get: opendir() failed: %s", strerror(errno));
		return (false);
	}
	ret = true;
	while ((dp = readdir(dirp)) != NULL)
	{
		if (!strcmp(dp->d_name, filename))
		{
			if (dp->d_type != DT_REG)
				ret = false;
			break;
		}
	}
	closedir(dirp);
	return (ret);
}

static t_ecode	sanitize_filename(char const *filename, t_env *e)
{
	t_ecode	err;

	if (ft_strchr(filename, '/') != NULL)
	{
		if ((err = message_send(E_MESSAGE_ERR, g_not_basename_err,
					sizeof(g_not_basename_err), e->csock)))
			return (err);
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	if (!is_regular_file_or_not_exist(filename, e))
	{
		if ((err = message_send(E_MESSAGE_ERR, g_not_regular_err,
					sizeof(g_not_regular_err), e->csock)))
			return (err);
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	return (E_ERR_OK);
}

/*
 * Do not allow relative paths in get. We cannot properly determine if the
 * target file is outside the root path.
 */
t_ecode	get_op_handler(t_message *msg, t_env *env)
{
	t_map	map;
	t_ecode	err;
	t_message	*begin;

	if (!msg->hd.size)
	{
		env->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	msg->payload[msg->hd.size - 1] = '\0';

	if ((err = sanitize_filename(msg->payload, env)))
		return (err);

	if ((err = file_map_rd(msg->payload, &map)))
	{
		env->log(env, "get %s: %s - %s",
				msg->payload, error_get_string(err), strerror(errno));
		return (message_send(E_MESSAGE_ERR,
					strerror(errno), ft_strlen(strerror(errno)), env->csock));
	}
	if ((err = message_send(E_MESSAGE_OK, &map.size, sizeof(map.size), env->csock)))
	{
		file_unmap(&map);
		env->should_quit = true;
		return (err);
	}
	if ((err = message_receive(&begin, env->csock)))
	{
		file_unmap(&map);
		env->should_quit = true;
		return (err);
	}
	if (begin->hd.op == E_MESSAGE_OK)
	{
		if (sock_raw_write(env->csock, map.data, map.size) < 0)
		{
			//critical
			file_unmap(&map);
			env->should_quit = true;
			message_destroy(begin);
			return (E_ERR_WRITE);
		}
		err = E_ERR_OK;
	}
	else if (begin->hd.op == E_MESSAGE_ERR)
		env->log(env, "client encountered an error, giving up on transfer\n");
	else
		err = E_ERR_UNEXPECTED_OP;

	message_destroy(begin);
	file_unmap(&map);
	return (err);
}
