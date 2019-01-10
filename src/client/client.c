/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/02 15:40:34 by tvallee           #+#    #+#             */
/*   Updated: 2018/12/17 15:56:25 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void usage(char *str)
{
	printf("Usage: %s <addr> <port>\n", str);
	exit(-1);
}

static int	err(char const *msg)
{
	printf("%s: %s\n", msg, strerror(errno));
	return (-1);
}

static int	create_client(char const *addr, int port)
{
	int		sock;
	struct protoent	*proto;
	struct sockaddr_in	sin;
	struct hostent	*host;

	proto = getprotobyname("tcp");
	host = gethostbyname(addr);
	if (proto == 0 || !host)
		return (err("error"));
	assert(host->h_length == 4);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (sock < 0)
		return (err("socket"));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = *(uint32_t *)host->h_addr;
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		return (err("connect"));
	return (sock);
}

static void	prompt(void)
{
	char const	s[] = "ftp $> "; //TODO: nice prompt with remote addr

	write(STDOUT_FILENO, s, sizeof(s) - 1);
}

int			loop(t_env *e)
{
	char			*ln;
	char			**args;
	int				ret;
	char const		*reason;
	
	e->should_quit = false;
	while (!e->should_quit)
	{
		prompt();
		if ((ret = get_next_line(STDIN_FILENO, &ln)) <= 0)
			break; // TODO Call exit command
		if ((args = ft_strsplit_fromtab(ln, " \f\n\r\t\v")) == NULL)
			return (err("malloc"));
		if (args[0] != NULL)
		{
			if (command_exec(args, &reason, e))
				printf("\n\033[92mSUCCESS\033[0m\n");
			else
				printf("\n\033[31mERROR\033[0m" ": %s: %s\n", args[0], reason);
		}
		free(ln);
		ft_freetab((void**)args);
	}
	return (ret);
}

int			main(int ac, char **av)
{
	t_bool	sane;
	int		port;
	int		ret;
	t_env	e;

	if (ac == 3)
		port = ft_atoi_sane(av[2], &sane);
	else
		sane = false;
	if (!sane)
	{
		usage(av[0]);
		return (1);
	}
	if ((e.csock = create_client(av[1], port)) < 0)
		return (1);
	ret = loop(&e);
	close(e.csock);
	return (ret);
}
