/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:18:53 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 18:13:33 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/*
** TODO:
** - max number of forks
** - mask and unmask signal handler for reap_children
*/

/*
** we might want to restore default signal disposition before calling
** connection_worker
*/

static void	attempt_new_connection(int cs,
		t_master_env *menv)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		perror("fork");
	else if (pid)
		menv->env.log(&menv->env, "spawned a new child %d\n", pid);
	else
	{
		close(menv->lsock);
		connection_worker(cs);
	}
}

/*
** FIXME: if a child process dies after the handler is unmasked and
** before the syscall, it will be waited for after the syscall
*/

int			main(int ac, char const *av[])
{
	unsigned int		cslen;
	struct sockaddr		csin;
	int					cs;
	t_master_env		menv;

	if (!master_init(&menv, ac, av))
		return (1);
	while (true)
	{
		reap_children(&menv.env);
		cslen = sizeof(csin);
		if ((cs = accept(menv.lsock, &csin, &cslen)) == -1)
		{
			if (errno != EINTR)
				perror("accept");
			continue ;
		}
		attempt_new_connection(cs, &menv);
		close(cs);
	}
	close(menv.lsock);
	return (0);
}
