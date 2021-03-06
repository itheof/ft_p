/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 14:16:56 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 19:06:46 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "libft/stdbool.h"

typedef enum	e_ecode
{
	E_ERR_OK = false,
	E_ERR_CLOSED,
	E_ERR_SERVER,
	E_ERR_CLIENT,
	E_ERR_INCOMPLETE_HEADER,
	E_ERR_INCOMPLETE_PAYLOAD,
	E_ERR_INVALID_PAYLOAD,
	E_ERR_UNEXPECTED_OP,
	E_ERR_UNIMPLEMENTED_OP,
	E_ERR_INVALID_OP,
	E_ERR_READ,
	E_ERR_WRITE,
	E_ERR_MMAP,
	E_ERR_FTRUNC,
	E_ERR_OPENDIR,
	E_ERR_CHDIR,
	E_ERR_GETCWD,
	E_ERR_FSTAT,
	E_ERR_MALLOC,
	E_ERR_OPEN,
	E_ERR_MAX,
}				t_ecode;

char const	*error_get_string(t_ecode e);

#endif
