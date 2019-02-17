/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 17:05:32 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 18:00:04 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static t_ecode		handle_response(t_message *msg, off_t *sizep, t_env *e)
{
	if (msg->hd.op == E_MESSAGE_OK)
	{
		if (msg->hd.size != sizeof(*sizep))
			return (E_ERR_INVALID_PAYLOAD);
		ft_memcpy(sizep, msg->payload, sizeof(*sizep));
		return (E_ERR_OK);
	}
	else if (msg->hd.op == E_MESSAGE_ERR)
	{
		if (msg->hd.size > 0 && msg->hd.size < INT_MAX)
			e->log(e, "server: %.*s", (int)msg->hd.size - 1, msg->payload);
		return (E_ERR_SERVER);
	}
	else
		return (E_ERR_UNEXPECTED_OP);
}

static t_ecode		get_file_size(char const *filename,
		off_t *sizep, char const **reason, t_env *e)
{
	t_message	*msg;
	t_ecode		err;

	if (!(err = message_send(E_MESSAGE_GET,
					filename, ft_strlen(filename) + 1, e->csock)))
		err = message_receive(&msg, e->csock);
	if (err)
	{
		e->should_quit = true;
		*reason = error_get_string(err);
		return (err);
	}
	err = handle_response(msg, sizep, e);
	*reason = error_get_string(err);
	message_destroy(msg);
	return (err);
}

static t_ecode		finalize_handshake(t_bool success,
		char const **reason, t_env *e)
{
	t_ecode	err;

	err = message_send(success ? E_MESSAGE_OK : E_MESSAGE_ERR,
			NULL, 0, e->csock);
	if (err)
	{
		*reason = error_get_string(err);
		e->should_quit = true;
		return (err);
	}
	else
		return (E_ERR_OK);
}

t_bool				exec_cmd_get(char *const *args, char const **reason, t_env *e)
{
	t_map	map;
	t_ecode	err;

	if ((err = get_file_size(args[1], &map.size, reason, e)))
		return (false);
	if ((err = file_map_wr(ft_basename(args[1]), map.size, &map))) 
	{
		e->log(e, "get: %s: %s", error_get_string(err), strerror(errno));
		finalize_handshake(false, reason, e);
		*reason = error_get_string(err);
		return (false);
	}
	if ((err = finalize_handshake(true, reason, e)))
	{
		file_unmap(&map);
		return (false);
	}
	if (sock_raw_read(e->csock, map.data, map.size) < 0)
	{
		e->log(e, "get: read(): %s", strerror(errno));
		file_unmap(&map);
		e->should_quit = true;
		*reason = error_get_string(E_ERR_READ);
		return (false);
	}
	file_unmap(&map);
	return (true);
}
