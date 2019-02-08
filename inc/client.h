/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:32 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/08 20:37:28 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "common.h"
# include "libft.h"

# define COMMANDS_LEN 12

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

t_bool			command_exec(char *const *args, char const **reason, t_env *e);

t_bool			exec_cmd_help(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_ping(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_pwd(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_lpwd(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_cd(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_lcd(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_ls(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_lls(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_get(char *const *args, char const **reason, t_env *e);
t_bool			exec_cmd_put(char *const *args, char const **reason, t_env *e);

#endif
