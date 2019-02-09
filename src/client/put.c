/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 16:51:14 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/09 16:55:02 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static t_ecode client_transfer(void const *map, off_t size, t_env *e)
{
	if (sock_raw_write(e->csock, map, size) < 0)
	{
		e->log(e, "put: write(): %s", strerror(errno));
		e->should_quit = true;
		return (E_ERR_WRITE);
	}
	return (E_ERR_OK);
}

static t_ecode	client_handshake(char const *filename, off_t size, t_env *e)
{
	t_ecode		err;
	t_message	*msg;

	e->should_quit = true;
	if ((err = message_send(E_MESSAGE_PUT, filename, ft_strlen(filename) + 1,
					e->csock)))
		return (err);
	if ((err = message_send(E_MESSAGE_PUT_SIZE, &size, sizeof(size), e->csock)))
		return (err);
	if ((err = message_receive(&msg, e->csock)))
		return (err);
	e->should_quit = false;
	if (msg->hd.op == E_MESSAGE_OK)
		err = E_ERR_OK;
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0 && msg->hd.size < INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
		err = E_ERR_SERVER;
	}
	else
		err = E_ERR_UNEXPECTED_OP;
	message_destroy(msg);
	return (err);
}

static t_ecode	prepare_file(char *path, int *dfd, struct stat *dbuf,
		void **dmap)
{
	int			saved_errno;

	if ((*dfd = open(path, O_RDONLY)) < 0)
		return (E_ERR_OPEN);
	if (fstat(*dfd, dbuf) < 0)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_FSTAT);
	}
	if ((*dmap = mmap(0, dbuf->st_size, PROT_READ, MAP_SHARED, *dfd, 0))
			== MAP_FAILED)
	{
		saved_errno = errno;
		close(*dfd);
		errno = saved_errno;
		return (E_ERR_MMAP);
	}
	return (E_ERR_OK);
}

t_bool			exec_cmd_put(char *const *args, char const **reason, t_env *e)
{
	int			fd;
	struct stat	buf;
	void		*map;
	t_ecode		err;

	if (!(err = prepare_file(args[1], &fd, &buf, &map)))
	{
		if (!(err = client_handshake(args[1], buf.st_size, e)))
			err = client_transfer(map, buf.st_size, e);
		munmap(map, buf.st_size);
		close(fd);
	}
	else
		e->log(e, "put: %s: %s", error_get_string(err), strerror(errno));
	if (err)
	{
		*reason = error_get_string(err);
		return (false);
	}
	return (true);
}
