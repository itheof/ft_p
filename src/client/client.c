/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/02 15:40:34 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 15:15:54 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static int	err(char const *msg)
{
	printf("%s: %s\n", msg, strerror(errno));
	return (-1);
}

static int	create_client(char const *addr, int port)
{
	struct protoent		*proto;
	struct hostent		*host;
	struct sockaddr_in	sin;
	int					sock;

	proto = getprotobyname("tcp");
	host = gethostbyname(addr);
	if (proto == 0 || !host)
		return (err("error"));
	assert(host->h_length == 4);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (sock < 0)
		return (err("socket"));
	else if (sock <= STDERR_FILENO)
	{
		printf("socket returned a standard FILENO. exiting to prevent hang\n");
		return (-1);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = *(uint32_t *)host->h_addr;
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		return (err("connect"));
	return (sock);
}

void		loop(t_env *e)
{
	char			*ln;
	char			**args;
	char const		*reason;

	e->should_quit = false;
	ln = NULL;
	while (!e->should_quit)
	{
		ft_strdel(&ln);
		write(STDOUT_FILENO, "ftp $> ", sizeof("ftp $> ") - 1);
		if ((e->ret = get_next_line(STDIN_FILENO, &ln)) <= 0)
			break ;
		if ((args = ft_strsplit_fromtab(ln, " \f\n\r\t\v")) == NULL)
		{
			e->ret = err("malloc");
			break ;
		}
		if (args[0] != NULL)
		{
			if (command_exec(args, &reason, e))
				printf("\n\033[92mSUCCESS\033[0m\n");
			else
				printf("\n\033[31mERROR\033[0m" ": %s: %s\n", args[0], reason);
		}
		ft_freetab((void**)args);
	}
	ft_strdel(&ln);
}

int			print(t_env const *e, const char *format, ...)
{
	va_list args;
	int		ret;

	(void)e;
	va_start(args, format);
	ret = vprintf(format, args);
	ret += printf("\n");
	va_end(args);
	return (ret);
}

int			main(int ac, char **av)
{
	t_bool	sane;
	int		port;
	t_env	e;

	e.log = print;
	if (ac == 3)
		port = ft_atoi_sane(av[2], &sane);
	else
		sane = false;
	if (!sane)
	{
		printf("Usage: %s <addr> <port>\n", av[0]);
		return (1);
	}
	if (!(e.cwd_path = getcwd(NULL, 0)))
	{
		perror("getcwd");
		return (1);
	}
	if ((e.csock = create_client(av[1], port)) < 0)
		return (1);
	loop(&e);
	close(e.csock);
	return (e.ret);
}
