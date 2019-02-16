/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 18:05:48 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 18:11:17 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static volatile sig_atomic_t	g_zombie_child_flag = 0;

static void						set_zombie_child_flag(int sig)
{
	(void)sig;
	g_zombie_child_flag = 1;
}

void							reap_children(t_env const *env)
{
	int		status;
	pid_t	pid;

	if (g_zombie_child_flag)
	{
		while ((pid = wait4(-1, &status, WNOHANG, NULL)) > 0)
			env->log(env, "process %d exit with status %d\n", pid, status);
		g_zombie_child_flag = 0;
	}
}

t_bool							set_signal_handler(void)
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
