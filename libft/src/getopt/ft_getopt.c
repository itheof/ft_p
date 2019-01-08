/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/30 13:49:50 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/20 11:47:11 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft/libc.h"
#include "libft/getopt.h"

static int	opt_get_type(const char *const *av,
		const char *optstring, t_opt *state)
{
	char	*addr;
	char	temp;

	temp = av[state->optind][state->optoff];
	state->optopt = temp;
	if ((addr = ft_strchr(optstring, temp)) == NULL)
	{
		if (av[state->optind][state->optoff + 1] != '\0')
			state->optoff += 1;
		else
		{
			state->optoff = 0;
			state->optind += 1;
		}
		if (state->opterr)
			ft_getopt_err(av[0], ": invalid option -- '", temp);
		return (E_OPT_TYPE_ERR);
	}
	else
		return ((addr[1]) == ':' ? E_OPT_TYPE_OPERAND : E_OPT_TYPE_SIMPLE);
}

static int	parse_operand_opt(int ac, const char *const *av,
		const char *optstring, t_opt *state)
{
	int c;

	c = av[state->optind][state->optoff];
	if (av[state->optind][state->optoff + 1] == '\0')
	{
		state->optarg = av[state->optind + 1];
		state->optind += 2;
		state->optoff = 0;
		if (state->optind > ac)
		{
			if (state->opterr && optstring[0] != ':')
				ft_getopt_err(av[0], ": option requires an argument -- '", c);
			return ((optstring[0] == ':') ? ':' : '?');
		}
	}
	else
	{
		state->optarg = av[state->optind] + state->optoff + 1;
		state->optind += 1;
		state->optoff = 0;
	}
	return (c);
}

static int	parse_simple_opt(const char *const *av, t_opt *state)
{
	int c;

	c = av[state->optind][state->optoff];
	if (av[state->optind][state->optoff + 1] != '\0')
		state->optoff += 1;
	else
	{
		state->optoff = 0;
		state->optind += 1;
	}
	return (c);
}

static int	is_opt_candidate(const char *str)
{
	return (str && str[0] == '-' && str[1]);
}

int			ft_getopt(int ac, const char *const *av, const char *optstring,
		t_opt *state)
{
	int opt_type;

	if (state->optind == 0)
		state->optind = 1;
	if (!state->optoff)
	{
		if (state->optind < ac && is_opt_candidate(av[state->optind]))
		{
			if (av[state->optind][1] == '-' && av[state->optind][2] == '\0')
				state->optind += 1;
			else
			{
				state->optoff = 1;
				return (ft_getopt(ac, av, optstring, state));
			}
		}
		return (-1);
	}
	opt_type = opt_get_type(av, optstring, state);
	if (opt_type == E_OPT_TYPE_OPERAND)
		return (parse_operand_opt(ac, av, optstring, state));
	else if (opt_type == E_OPT_TYPE_SIMPLE)
		return (parse_simple_opt(av, state));
	return ('?');
}
