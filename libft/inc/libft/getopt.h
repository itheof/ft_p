/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_opts.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/15 10:16:50 by tvallee           #+#    #+#             */
/*   Updated: 2018/02/20 11:39:01 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_H
# define FT_GETOPT_H

# define OPT_INIT(o) ((o) = (t_opt){NULL, 0, 0, 0, 0})

typedef struct	s_opt
{
	const char	*optarg;
	int			opterr;
	int			optind;
	int			optopt;
	int			optoff;
}				t_opt;

enum			e_opt_type
{
	E_OPT_TYPE_ERR,
	E_OPT_TYPE_SIMPLE,
	E_OPT_TYPE_OPERAND
};

int				ft_getopt(int ac, const char *const *av,
		const char *optstring, t_opt *state);
void			ft_getopt_err(const char *name, const char *reason,
		char invalid);

#endif
