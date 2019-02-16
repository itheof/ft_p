/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 17:59:22 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/16 18:15:42 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static t_bool	parse_cl_fail(t_master_env *menv, char const *name)
{
	menv->env.log(&menv->env, "Usage: %s [-d chbasedir] PORT\n", name);
	return (false);
}

t_bool			parse_cl(t_master_env *menv, int ac, char const *av[])
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
