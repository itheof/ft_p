/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 16:15:47 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 16:15:49 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "error.h"

/*
** for internal use only // not for communication
*/

static char const	*(g_err_desc[]) = {
	[E_ERR_OK] = "success",
	[E_ERR_CLOSED] = "connection closed by peer",
	[E_ERR_SERVER] = "server error",
	[E_ERR_INCOMPLETE_HEADER] = "received partial ft_p header",
	[E_ERR_INCOMPLETE_PAYLOAD] = "received partial ft_p payload",
	[E_ERR_INVALID_PAYLOAD] = "received invalid payload",
	[E_ERR_UNEXPECTED_OP] = "received unexpected opcode as a response",
	[E_ERR_UNIMPLEMENTED_OP] = "received unimplemented opcode",
	[E_ERR_INVALID_OP] = "received invalid opcode",
	[E_ERR_READ] = "read error",
	[E_ERR_WRITE] = "write error",
	[E_ERR_MMAP] = "mmap error",
	[E_ERR_FTRUNC] = "ftruncate error",
	[E_ERR_OPENDIR] = "opendir error",
	[E_ERR_CHDIR] = "chdir error",
	[E_ERR_GETCWD] = "getcwd error",
	[E_ERR_FSTAT] = "fstat error",
	[E_ERR_MALLOC] = "malloc error",
	[E_ERR_OPEN] = "open error",
};

char const	*error_get_string(t_ecode e)
{
	assert(e >= 0 && e < E_ERR_MAX
			&& e < sizeof(g_err_desc) / sizeof(g_err_desc[0]));
	return (g_err_desc[e]);
}
