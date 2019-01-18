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

static t_bool	parse_cl_fail(t_master_env *menv, char const *name)
{
	menv->env.log(&menv->env, "Usage: %s [-d chbasedir] PORT\n", name);
	return (false);
}

static t_bool	parse_cl(t_master_env *menv, int ac, char const *av[])
{
	t_bool	sane;
	t_opt	state;
	int		opt;

	ft_memset(&state, 0, sizeof(state));
	while ((opt = ft_getopt(ac, av, "d:", &state)) != -1)
	{
		if (opt == '?')
			return (parse_cl_fail(menv, av[0]));
		else if (opt == 'd')
			menv->base_dir = state.optarg;
	}
	if (ac - state.optind != 1)
		return (parse_cl_fail(menv, av[0]));
	menv->port = ft_atoi_sane(av[state.optind], &sane);
	if (!sane)
		return (parse_cl_fail(menv, av[0]));
	return (true);
}

static t_bool	setup_basedir(t_master_env *menv)
{
	char	*path;
	//TODO: this sets PWD to the realpath of the basedir when it should instead
	// try to sanitize the logical path that PWD can represent
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
	if (!(path = getcwd(NULL, 0)))
	{
		menv->env.log(&menv->env, "getcwd: %s", strerror(errno));
		return (false);
	}
	if (setenv("PWD", path, 1) != 0)
	{
		menv->env.log(&menv->env, "setenv: %s", strerror(errno));
		free(path);
		return (false);
	}
	free(path);
	return (true);
}

int	master_init(t_master_env *menv, int ac, char const *av[])
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
	
	// allow killing the master process and its children through the same PG
	// an interactive shell already setpgrp's before exec'ing. enforce this
	setpgrp();
	return (true);
}
