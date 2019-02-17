/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 16:51:51 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 17:59:15 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	g_not_basename_err[] = "put: file must be a single path "
"component, relative to the current directory";

/*
** TODO: enhance to -> is_same_dir
*/

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

t_ecode			put_op_handler(t_message *msg, t_env *e)
{
	t_map	map;
	t_ecode	err;

	if (!msg->hd.size)
	{
		e->should_quit = true;
		return (E_ERR_INVALID_PAYLOAD);
	}
	msg->payload[msg->hd.size - 1] = '\0';
	if ((err = get_file_size(&map.size, e)))
		return (err);
	if ((err = sanitize_filename(msg->payload, e)))
		return (err);
	if ((err = file_map_wr(msg->payload, map.size, &map)))
	{
		e->log(e, "put: %s: %s", error_get_string(err), strerror(errno));
		if (message_send(E_MESSAGE_ERR, strerror(errno),
					ft_strlen(strerror(errno)), e->csock) != E_ERR_OK)
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
		err = server_transfer(map.data, map.size, e);
	file_unmap(&map);
	return (err == E_ERR_OK);
}
