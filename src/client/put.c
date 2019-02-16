/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 16:51:14 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 13:07:03 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static t_ecode	client_transfer(void const *map, off_t size, t_env *e)
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

t_bool			exec_cmd_put(char *const *args, char const **reason, t_env *e)
{
	int			fd;
	off_t		size;
	void		*map;
	t_ecode		err;

	if (!(err = file_map_rd(args[1], &fd, &size, &map)))
	{
		if (!(err = client_handshake(args[1], size, e)))
			err = client_transfer(map, size, e);
		else
			e->log(e, "put: could not handshake");
		file_unmap(fd, size, map);
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
