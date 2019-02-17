/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/02 15:40:34 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/17 18:16:44 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	wrap_command_exec(char **args, t_env *e)
{
	char const		*reason;

	if (command_exec(args, &reason, e))
		printf("\n\033[92mSUCCESS\033[0m\n");
	else
		printf("\n\033[31mERROR\033[0m" ": %s: %s\n", args[0], reason);
}

static void	loop(t_env *e)
{
	char			*ln;
	char			**args;

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
			printf("malloc: %s\n", strerror(errno));
			e->ret = -1;
			break ;
		}
		if (args[0] != NULL)
			wrap_command_exec(args, e);
		ft_freetab((void**)args);
	}
	ft_strdel(&ln);
}

static int	print(t_env const *e, const char *format, ...)
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
