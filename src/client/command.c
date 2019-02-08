/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 14:17:48 by tvallee           #+#    #+#             */
/*   Updated: 2019/02/08 14:46:14 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static t_bool	exec_not_impl(char const **reason)
{
	*reason = "exec function not implemented yet";
	return (false);
}

static t_bool	exec_cmd_quit(char * const *args, char const **reason, t_env *e)
{
	(void)args;
	(void)reason;
	e->should_quit = true;
	return (true);
}

t_command g_commands[COMMANDS_LEN] = {
	{
		.name = "ping",
		.desc = "request a pong from remote",
		.args = NULL,
		.nargs = 0,
		.exec = exec_cmd_ping
	},
	{
		.name = "help",
		.desc = "show help message for available commands",
		.args = NULL,
		.nargs = 0, //TODO: Variadic arguments
		.exec = exec_cmd_help
	},
	{
		.name = "quit",
		.desc = "quit the client",
		.args = NULL,
		.nargs = 0,
		.exec = exec_cmd_quit
	},
	{
		.name = "exit",
		.desc = "alias for `quit'",
		.args = NULL,
		.nargs = 0,
		.exec = exec_cmd_quit
	},
	{
		.name = "ls",
		.desc = "list remote files for the current directory",
		.args = NULL,
		.nargs = 0,
		.exec = exec_cmd_ls
	},
	{
		.name = "cd",
		.desc = "change the remote current directory",
		.args = NULL,
		.nargs = 1,
		.exec = exec_cmd_cd,
	},
	{
		.name = "get",
		.desc = "download file from server",
		.args = NULL,
		.nargs = 1,
		.exec = exec_cmd_get,
	},
	{
		.name = "put",
		.desc = "upload file to server",
		.args = NULL,
		.nargs = 1,
	},
	{
		.name = "pwd",
		.desc = "print the remote current directory",
		.args = NULL,
		.nargs = 0,
		.exec = exec_cmd_pwd
	},
	{
		.name = "lcd",
		.desc = "change the local current directory",
		.args = NULL,
		.nargs = 1,
		.exec = exec_cmd_lcd
	},
	{
		.name = "lpwd",
		.desc = "print the local current directory",
		.args = NULL,
		.nargs = 0,
		.exec = exec_cmd_lpwd
	},
	{
		.name = "lls",
		.desc = "list local files for the current directory",
		.args = NULL,
		.nargs = 0,
		.exec = exec_cmd_lls
	}
};

static t_command const	*command_match(char const *name)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_commands) / sizeof(*g_commands))
	{
		if (!ft_strcmp(name, g_commands[i].name))
			return (g_commands + i);
		i++;
	}
	return (NULL);
}

t_bool	command_exec(char * const *args, char const **reason, t_env *e)
{
	t_command const	*match;

	*reason = "programming error: *reason should be set in case of error";
	if (!(match = command_match(args[0])))
	{
		*reason = ("unknown command");
		return (false);
	}
	if (ft_tablen((void**)args + 1) != match->nargs)
	{
		*reason = "bad usage (use 'help <command>')";
		return (false);
	}
	if (!match->exec)
		return(exec_not_impl(reason));
	return(match->exec(args, reason, e));
}
