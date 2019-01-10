/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:32 by tvallee           #+#    #+#             */
/*   Updated: 2018/12/17 14:25:50 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "common.h"
# include "libft.h"

# define COMMANDS_LEN 11

typedef struct	s_arg
{
	char const	*name;
	char const	*desc;
}				t_arg;

typedef struct	s_command
{
	char const	*name;
	char const	*desc;
	t_arg const	*(args);
	size_t		nargs;
	t_bool		local;
	t_bool		(*exec)(char * const *args,
			char const **reason, t_env *e);
}				t_command;

t_bool			command_exec(char * const *args,
		char const **reason, t_env *e);

t_bool			exec_cmd_help(char * const *args,
		char const **reason, t_env *e);
t_bool			exec_cmd_ping(char * const *args,
		char const **reason, t_env *e);

#endif
