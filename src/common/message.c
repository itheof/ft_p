#include "common.h"

/*
** \desc guarantees either complete payload and valid op or returns NULL
*/

t_ecode		message_receive(t_message **dst, int sock)
{
	t_message_hd	hd;
	ssize_t			received;

	received = sock_raw_read(sock, &hd, sizeof(hd));
	if (received == 0)
		return (E_ERR_CLOSED);
	else if (received < 0)
		return (E_ERR_READ);
	else if (received < (ssize_t)sizeof(hd))
		return (E_ERR_INCOMPLETE_HEADER);
	if (!(hd.op < E_MESSAGE_MAX))
		return (E_ERR_INVALID_OP);
	if ((*dst = malloc(sizeof(hd) + hd.size)) == NULL)
		return (E_ERR_MALLOC);
	(*dst)->hd = hd;
	received = sock_raw_read(sock, (*dst)->payload, (*dst)->hd.size);
	if (received < 0)
		return (E_ERR_READ);
	if ((size_t)received < (*dst)->hd.size)
	{
		free(*dst);
		return (E_ERR_INCOMPLETE_PAYLOAD);
	}
	return (E_MESSAGE_OK);
}

void		message_destroy(t_message *msg)
{
	free(msg);
}

t_ecode		message_send(t_message_op op, void *data, size_t nbyte, int sock)
{
	t_message_hd	hd;
	ssize_t			written;

	hd.size = nbyte;
	hd.op = op;
	written = sock_raw_write(sock, &hd, sizeof(hd));
	if (written < 0)
		return (E_ERR_WRITE);
	else if ((size_t)written < sizeof(hd))
		return (E_ERR_CLOSED);
	written = sock_raw_write(sock, data, nbyte);
	if (written < 0)
		return (E_ERR_WRITE);
	else if ((size_t)written < nbyte)
		return (E_ERR_CLOSED);
	return (E_ERR_OK);
}
