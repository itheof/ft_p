/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 16:39:54 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/26 15:01:17 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_H
# define BUFFER_H

# include <stdlib.h>
# include "libft/stdbool.h"

/*
** BUFFER_BASE_SIZE must be a power of two
*/

# ifndef BUFFER_BASE_SIZE
#  define BUFFER_BASE_SIZE 4096
# endif

typedef struct	s_buffer
{
	char	*str;
	size_t	size;
	size_t	len;
}				t_buffer;

t_bool			buffer_init(t_buffer *buf);
t_bool			buffer_init_size(t_buffer *buf, size_t size);
t_bool			buffer_init_with(t_buffer *buf, const char *str);
t_bool			buffer_reserve(t_buffer *buf, size_t avail);

/*
** Room for optimizing
*/

t_bool			buffer_ncat(t_buffer *buf, const char *str, size_t n);
t_bool			buffer_cat(t_buffer *buf, const char *str);
t_bool			buffer_cat_num(t_buffer *buf, size_t n);

/*
** Unsafe functions: no checking is performed
*/

void			buffer_deinit(t_buffer *buf);

#endif
