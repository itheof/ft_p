/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 16:51:51 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/09 18:17:12 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	g_not_basename_err[] = "put: file must be a single path "
"component, relative to the current directory";

static const char	g_exists_err[] = "put: file exists";

//TODO: enhance to -> is_same_dir
static t_bool	is_basename(char const *path)
{
	if (!ft_strcmp(".", path))
		return (false);
	else if (!ft_strcmp("..", path))
		return (false);
	return (ft_strchr(path, '/') == NULL);
}

static t_bool	file_exists(char const *filename, t_env *e)
{
	struct dirent	*dp;
	t_bool			ret;
	DIR				*dirp;

	dirp = opendir(".");
	if (dirp == NULL)
	{
		e->log(e, "put: opendir() failed: %s", strerror(errno));
		return (false);
	}
	ret = true;
	while ((dp = readdir(dirp)) != NULL)
	{
		if (!strcmp(dp->d_name, filename))
		{
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

	if (!is_basename(filename))
	{
		if ((err = message_send(E_MESSAGE_ERR, g_not_basename_err,
					sizeof(g_not_basename_err), e->csock)))
			return (err);
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	if (!file_exists(filename, e))
	{
		if ((err = message_send(E_MESSAGE_ERR, g_exists_err,
					sizeof(g_exists_err), e->csock)))
			return (err);
		else
			return (E_ERR_INVALID_PAYLOAD);
	}
	return (E_ERR_OK);
}

static t_ecode	get_file_size(off_t *dsize, t_env *e)
{
	t_message	*msg;
	t_ecode		err;

	if ((err = message_receive(&msg, e->csock)))
	{
		e->should_quit = true;
		return (err);
	}
	if (msg->hd.size != sizeof(*dsize))
	{
		message_destroy(msg);
		e->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	*dsize = *((off_t*)msg->payload);
	message_destroy(msg);
	return (E_ERR_OK);
}

static t_ecode	server_transfer(void *map, off_t size, t_env *e)
{
	if (sock_raw_read(e->csock, map, size) < 0)
	{
		e->log(e, "put: read(): %s", strerror(errno));
		e->should_quit = true;
		return (E_ERR_READ);
	}
	return (E_ERR_OK);
}

t_ecode	put_op_handler(t_message *msg, t_env *e)
{
	int 	fd;
	off_t	size;
	void	*map;
	t_ecode	err;

	if (!msg->hd.size)
	{
		e->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	msg->payload[msg->hd.size - 1] = '\0';
	if ((err = sanitize_filename(msg->payload, e)))
		return (err);
	if ((err = get_file_size(&size, e)))
		return (err);
	if ((err = file_map_wr(msg->payload, size, &fd, &map)))
	{
		e->log(e, "put: %s: %s", error_get_string(err), strerror(errno));
		if (message_send(E_MESSAGE_ERR, strerror(err),
					ft_strlen(strerror(err)), e->csock) != E_ERR_OK)
		{
			e->should_quit = true;
			return (E_ERR_WRITE);
		}
		else
			return (err);
	}
	if ((err = message_send(E_MESSAGE_OK, NULL, 0, e->csock)))
		e->should_quit = true;
	else
		err = server_transfer(map, size, e);
	munmap(map, size);
	close(fd);
	return (err == E_ERR_OK);
}
