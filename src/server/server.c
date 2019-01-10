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
static sig_atomic_t	g_zombie_child_flag = 0;

static void	set_zombie_child_flag(int sig)
{
	(void)sig;
	g_zombie_child_flag = 1;
}

static void	reap_children(void) //FIXME
{
	int		status;
	pid_t	pid;

	//TODO mask handler
    while ((pid = wait4(-1, &status, WNOHANG, NULL)) > 0)
	{
		print_header(0);
		printf("process %d exit with status %d\n", pid, status);
    }
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

static void		attempt_new_connection(int cs, t_env *env)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		perror("fork");
	else if (pid)
	{
		print_header(env->pid);
		printf("spawned a new child %d\n", pid);
	}
	else
	{
		connection_worker(env, cs);
	}
}

int				main(int ac, char const *av[])
{
	unsigned int	cslen;
	struct sockaddr	csin;
	int				cs;
	t_env			env;

	if (!init(&env, ac, av))
		return (1);
	while (true)
	{
		if (g_zombie_child_flag)
			reap_children();
		//FIXME: if a child process dies after the handler is unmasked and
		//       before the syscall, it will be waited for after the syscall
		if ((cs = accept(env.lsock, &csin, &cslen)) == -1)
		{
			if (errno != EINTR)
				perror("accept");
			continue ;
		}
		attempt_new_connection(cs, &env);
		close(cs);
    }
	close(env.lsock);
	return (0);
}
