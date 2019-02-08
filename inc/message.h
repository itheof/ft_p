/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 14:17:09 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/08 14:17:38 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_H
# define MESSAGE_H

# include "error.h"
# include "common.h"

typedef enum	e_message_op
{
	E_MESSAGE_OK,
	E_MESSAGE_ERR,
	E_MESSAGE_PING,
	E_MESSAGE_LS,
	E_MESSAGE_CD,
	E_MESSAGE_GET,
	E_MESSAGE_PUT,
	E_MESSAGE_PWD,

	E_MESSAGE_MAX
}				t_message_op;

typedef struct	s_message_hd
{
	t_message_op	op;
	uint64_t		size;
}				t_message_hd;

typedef struct	s_message
{
	t_message_hd	hd;
	char			payload[];
}				t_message;

t_ecode			message_receive(t_message **dst, int sock);

t_ecode			message_send(t_message_op op, void const *data, size_t nbyte,
		int sock);

t_ecode			message_send_unknown_err(int sock, int errnum);
void			message_destroy(t_message *msg);

#endif
