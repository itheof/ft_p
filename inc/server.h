/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:18 by tvallee           #+#    #+#             */
/*   Updated: 2018/12/17 16:24:22 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "common.h"
# include <signal.h>
# include <unistd.h>

typedef struct	s_env {
	int 	lsock;
	pid_t	pid;
}				t_env;

/*
** worker.c
*/

void	connection_worker(t_env *env, int cs);

/*
** misc.c
*/

void	print_header(pid_t pid);

/*
** init.c
*/

int		init(t_env *env, int ac, char const *av[]);

/*
** server.c
*/

t_bool	set_signal_handler(void);

#endif
