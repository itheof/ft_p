/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 17:05:32 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 15:16:39 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static char const	*ft_basename(char const *path)
{
	char const	*dir;

	dir = ft_strrchr(path, '/');
	if (dir)
		return (dir + 1);
	else
		return (path);
}

static t_bool	get_file_size(char const *filename, off_t *sizep,
		char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;
	t_bool		ret;

	if ((err = message_send(E_MESSAGE_GET,
					filename, ft_strlen(filename) + 1, e->csock)))
		return (command_abort(err, reason, &e->should_quit));
	if ((err = message_receive(&msg, e->csock)))
		return (command_abort(err, reason, &e->should_quit));
	ret = true;
	// not required. Makes sure struct stat.st_size is off_t
	assert(sizeof(*sizep) == sizeof(((struct stat *)NULL)->st_size));
	if (msg->hd.op == E_MESSAGE_OK && msg->hd.size == sizeof(*sizep))
	{
		ft_memcpy(sizep, msg->payload, sizeof(*sizep));
		ret = false;
	}
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0 && msg->hd.size < INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
		*reason = error_get_string(E_ERR_SERVER);
	}
	else
		*reason = error_get_string(E_ERR_UNEXPECTED_OP);
	message_destroy(msg);
	return (ret);
}

t_bool	exec_cmd_get(char *const *args, char const **reason, t_env *e)
{
	char const	*local_path;

	off_t	size;
	t_bool	ret;
	int		fd;
	void	*map;
	t_ecode	err;

	if ((ret = get_file_size(args[1], &size, reason, e)))
		return (false);
	local_path = ft_basename(args[1]);
	if ((err = file_map_wr(local_path, size, &fd, &map))) 
	{
		e->log(e, "get: %s: %s", error_get_string(err), strerror(errno));
		e->should_quit = true;
		//TODO v: we need to read what the server writes to keep in sync
		//APPEND AN OK MESSAGE HANDSHAKE TO SOLVE THIS
		*reason = error_get_string(err);
		return (false);
	}
	if (sock_raw_read(e->csock, map, size) < 0)
	{
		e->log(e, "get: read(): %s", strerror(errno));
		file_unmap(fd, size, map);
		e->should_quit = true;
		*reason = error_get_string(E_ERR_READ);
		return (false);
	}
	file_unmap(fd, size, map);
	return (true);
}
