/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 17:51:14 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 18:41:26 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int		log_with_pid(t_env const *env, char const *format, ...)
{
	va_list args;
	int		ret;

	(void)env;
	assert(env->pid == 0);
	ret = printf("\033[31m" "master" "\033[0m" "%*c: ", LOG_PADDING, ' ');
	va_start(args, format);
	ret += vprintf(format, args);
	ret += printf("\n");
	va_end(args);
	return (ret);
}

static int		get_socket(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
	{
		perror("socket");
		return (-1);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		perror("bind");
	else if (listen(sock, 42) == -1)
		perror("listen");
	else
		return (sock);
	close(sock);
	return (-1);
}

/*
** TODO: this sets PWD to the realpath of the basedir when it should instead
** try to sanitize the logical path that PWD can represent
*/

static t_bool	setup_basedir(t_master_env *menv)
{
	char	*path;
	t_bool	ret;

	if (menv->base_dir)
	{
		if (chdir(menv->base_dir) != 0)
		{
			menv->env.log(&menv->env, "chdir(\"%s\"): %s", menv->base_dir,
					strerror(errno));
			return (false);
		}
		else
			menv->env.log(&menv->env, "successfully changed directory to %s",
					menv->base_dir);
	}
	ret = false;
	path = NULL;
	if (!(path = getcwd(NULL, 0)))
		menv->env.log(&menv->env, "getcwd: %s", strerror(errno));
	else if (setenv("PWD", path, 1) != 0)
		menv->env.log(&menv->env, "setenv: %s", strerror(errno));
	else
		ret = true;
	return (ret);
}

/*
** v allow killing the master process and its children through the same PG
** an interactive shell already setpgrp's before exec'ing. enforce this
*/

int				master_init(t_master_env *menv, int ac, char const *av[])
{
	menv->env.pid = 0;
	menv->env.log = log_with_pid;
	menv->base_dir = NULL;
	if (!parse_cl(menv, ac, av))
		return (false);
	if (!setup_basedir(menv))
		return (false);
	if ((menv->lsock = get_socket(menv->port)) < 0)
		return (false);
	if (!set_signal_handler())
		return (false);
	menv->env.log(&menv->env, "listening on port %d\n", menv->port);
	setpgrp();
	return (true);
}
