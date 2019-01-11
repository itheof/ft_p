#include "server.h"

static int	log_with_pid(t_env const *env, char const *format, ...)
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

int	master_init(t_master_env *menv, int ac, char const *av[])
{
	t_bool		sane;
	int			port;

	menv->env.log = log_with_pid;
	if (ac == 2)
		port = ft_atoi_sane(av[1], &sane);
	else
		sane = false;
	if (!sane)
	{
		printf("Usage: %s PORT\n", av[0]);
		return (false);
	}
	if ((menv->lsock = get_socket(port)) < 0)
		return (false);
	menv->env.pid = 0;
	if (!set_signal_handler())
		return (false);
	menv->env.log(&menv->env, "listening on port %d\n", port);
	
	// allow killing the master process and its children through the same PG
	// an interactive shell already setpgrp's before exec'ing. enforce this
	setpgrp();
	return (true);
}
