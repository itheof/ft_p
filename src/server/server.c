/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:18:53 by tvallee           #+#    #+#             */
/*   Updated: 2018/12/17 16:26:31 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

// TODO limit max forks
//
static volatile sig_atomic_t	g_zombie_child_flag = 0;

static void	set_zombie_child_flag(int sig)
{
	(void)sig;
	g_zombie_child_flag = 1;
}

static void	reap_children(t_env const *env) //FIXME
{
	int		status;
	pid_t	pid;

	//TODO mask handler
    while ((pid = wait4(-1, &status, WNOHANG, NULL)) > 0)
		env->log(env, "process %d exit with status %d\n", pid, status);
	g_zombie_child_flag = 0;
	//TODO unmask handler
	
}

t_bool	set_signal_handler(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_handler = set_zombie_child_flag;
	act.sa_flags = SA_NOCLDSTOP;

	if (sigaction(SIGCHLD, &act, NULL) == -1)
	{
		perror("sigaction");
		return (false);
	}
	return (true);
}

static void		attempt_new_connection(int cs, t_master_env *menv)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		perror("fork");
	else if (pid)
		menv->env.log(&menv->env, "spawned a new child %d\n", pid);
	else
	{
		close(menv->lsock);
		//we might want to restore default signal disposition
		connection_worker(cs);
	}
}

int				main(int ac, char const *av[])
{
	unsigned int		cslen;
	struct sockaddr		csin;
	int					cs;
	t_master_env		menv;

	if (!master_init(&menv, ac, av))
		return (1);
	while (true)
	{
		if (g_zombie_child_flag)
			reap_children(&menv.env);
		//FIXME: if a child process dies after the handler is unmasked and
		//       before the syscall, it will be waited for after the syscall
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
