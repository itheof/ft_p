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

#include "common.h"
#include "serveur.h"
#include <unistd.h>

#define LOG_PADDING 8
// TODO limit max forks

static void	print_header(pid_t pid)
{
	pid_t	tmp;
	int		i;

	tmp = pid;
	i = sizeof("()") - sizeof("");
	while (tmp > 0)
	{
		tmp /= 10;
		i++;
	}
	assert(LOG_PADDING >= i);
	if (pid) // this can be optimized obviously
		printf("worker(%d)%*c: ", pid, LOG_PADDING - i, ' ');
	else
		printf("\033[31m" "master" "\033[0m" "%*c: ", LOG_PADDING, ' ');
}


static void	reap_child(int sig)
{
    int 	save_errno;
	int		status;
	pid_t	pid;

	(void)sig;
    save_errno = errno;
    while ((pid = wait4(-1, &status, WNOHANG, NULL)) > 0)
	{
		print_header(0); //UNSAFE
		printf("process %d exit with status %d\n", pid, status); //UNSAFE
    }
    errno = save_errno;
}

static int	get_socket(int port) // TODO error handling return -1
{
	int	sock;
	struct protoent	*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		perror("bind");
		exit(2);
	}
	listen(sock, 42);
	return (sock);
}

static void	handle_connection(t_env *env, int cs)
{
	t_message	*msg;
	t_ecode		err;

	close(env->lsock);

	/* Work */
	while (!(err = message_receive(&msg, cs)))
	{
		switch (msg->hd.op) {
		case E_MESSAGE_PING:
			message_send(E_MESSAGE_OK, NULL, 0, cs);
			print_header(env->pid);
			printf("pong\n");
			break;
		default:
			print_header(env->pid);
			printf("received unimplemented op %d, skipping\n", msg->hd.op);
			break;
		}
		message_destroy(msg);
	}
	//TODO handle err code
	print_header(env->pid);
	printf("%s\n", error_get_string(err));

	close(cs);
	exit(EXIT_SUCCESS);
}

static int	init(t_env *env, int ac, char const *av[])
{
	t_bool		sane;
	int			port;

	if (ac == 2)
		port = ft_atoi_sane(av[1], &sane);
	else
		sane = false;
	if (!sane)
	{
		printf("Usage: %s PORT\n", av[0]);
		return (false);
	}
	if ((env->lsock = get_socket(port)) < 0)
		return (false);
	env->pid = 0;
	if (signal(SIGCHLD, reap_child) == SIG_ERR)
	{
		perror("signal");
		return (false);
	}
	print_header(0);
	printf("listening on port %d\n", port);
	return (true);
}

int				main(int ac, char const *av[])
{
	unsigned int		cslen;
	struct sockaddr_in	csin;
	int					cs;
	pid_t				pid;
	t_env				env;

	if (!init(&env, ac, av))
		return (1);
	setpgrp();
	while (true)
	{
		if ((cs = accept(env.lsock, (struct sockaddr*)&csin, &cslen)) == -1)
		{
			perror("accept");
			continue ;
		}
		if ((pid = fork()) < 0)
			perror("fork");
		else if (!pid)
		{
			env.pid = getpid();
			handle_connection(&env, cs);
		}
		else
		{
			print_header(env.pid);
			printf("spawned a new child %d\n", pid);
		}
		close(cs);
    }
	close(env.lsock);
	return (0);
}
