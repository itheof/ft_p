/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:48 by tvallee           #+#    #+#             */
/*   Updated: 2018/12/17 15:57:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# define _DEFAULT_SOURCE
# include <string.h>
# include <assert.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>

#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "libft.h"
#include "error.h"
#include "message.h"

int			ft_atoi_sane(char const *s, t_bool *sane);

/* Those functions block until the socket is closed on the other end or nbytes
** are read/written
*/

ssize_t		sock_raw_read(int fildes, void *buf, ssize_t nbyte);
ssize_t		sock_raw_write(int fildes, void *buf, ssize_t nbyte);

#endif
